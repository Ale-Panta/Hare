#include "EditorLayer.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Hare
{
	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), 
		m_CameraController(1280.0f / 720.0f, true), 
		m_ViewportSize(1280.0f, 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		HR_PROFILE_FUNCTION();

		m_SpreadSheet	= Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		m_TextureStair	= SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(7, 6), vec2(128.0f, 128.0f));
		m_TextureBarrel	= SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(8, 2), vec2(128.0f, 128.0f));
		m_TextureTree	= SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(2, 1), vec2(128.0f, 128.0f), vec2(1, 2));

		FramebufferSpecification fbSpecification;
		fbSpecification.Width = 1280.0f;
		fbSpecification.Height = 720.0f;
		m_Framebuffer = Framebuffer::Create(fbSpecification);

		m_ActiveScene = CreateRef<Scene>();

		// Entity
		auto square = m_ActiveScene->CreateEntity("Marco");
		square.AddComponent<SpriteRendererComponent>(vec4(1.0f));
		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

		m_SecondCamera = m_ActiveScene->CreateEntity("Clip-Space Camera");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		cc.Primary = false;

#if PARTICLE
		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 5.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };
#endif
	}

	void EditorLayer::OnDetach()
	{
		HR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		HR_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);


		// Reset stats here.
		Renderer2D::ResetStats();

		// Renderer
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// Update scene.
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HR_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) 
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGui::Begin("Setting");
		// Display stats.
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.Drawcalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (m_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			// Get the color property from the SpriteRendererComponent of our square.
			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;

			// Set the color of the square.
			ImGui::ColorEdit4("SquareColor", value_ptr(squareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		if (ImGui::Checkbox("Use Primary Camera", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}
		ImGui::End();	// End ImGui::Begin("Setting")


		// Customize window style. Push...
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


		ImGui::Begin("Viewport");
		// Is viewport focused...
		m_ViewportFocused = ImGui::IsWindowFocused();

		// Is viewport hovered...
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBLockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((vec2*)&viewportPanelSize) && m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
		{
			m_ViewportSize = vec2(viewportPanelSize.x, viewportPanelSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		uint32_t id = m_Framebuffer->GetColorAttachmentRenderID();

		// Display or color the viewport onto the texture.
		ImGui::Image((void*)id, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();	// End viewport
		

		// Customize window style. Pop...
		ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);


		ImGui::End();	// End dock space
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}