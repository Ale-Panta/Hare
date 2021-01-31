#include "EditorLayer.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Hare
{
	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_CameraController(1200.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		HR_PROFILE_FUNCTION();

		m_SpreadSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		m_TextureStair = SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(7, 6), vec2(128.0f, 128.0f));
		m_TextureBarrel = SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(8, 2), vec2(128.0f, 128.0f));
		m_TextureTree = SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(2, 1), vec2(128.0f, 128.0f), vec2(1, 2));

		FramebufferSpecification fbSpecification;
		fbSpecification.Width = 1280.0f;
		fbSpecification.Height = 720.0f;
		m_Framebuffer = Framebuffer::Create(fbSpecification);

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
		{
			HR_PROFILE_SCOPE("Render Preparation");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
		}

#if 0
		{
			static float rotation = 0.0f;
			rotation += ts * 20;

			HR_PROFILE_SCOPE("Render Draw");

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad(vec2(0.0f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.4f, 0.5f, 1.0f));
			Renderer2D::DrawQuad(vec2(0.5f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.1f, 0.8f, 1.0f));
			Renderer2D::DrawQuad(vec3(0.0f), vec2(10.0f, 10.0f), m_Texture, 5.0f, vec4(0.3f, 0.2f, 0.5f, 1.0f));
			Renderer2D::DrawQuad(vec3(0.0f, 0.0f, 0.1f), vec2(5.0f, 5.0f), m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
			Renderer2D::DrawRotatedQuad(vec3(-1.0f, -1.0f, 0.2f), vec2(2.0f, 2.0f), radians(37.0f), m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
			Renderer2D::DrawRotatedQuad(vec3(1.0f, 0.0f, 0.3f), vec2(2.0f, 2.0f), radians(rotation), vec4(0.8f, 0.8f, 0.8f, 1.0f));
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
					Renderer2D::DrawQuad(vec3(x, y, 0.1f), vec2(0.45f, 0.45f), color);
				}
			}
			Renderer2D::EndScene();
		}
#endif

#if PARTICLE
		if (Input::IsMouseButtonPressed(HR_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Input::GetMousePosition();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderer2D::DrawQuad(vec3(0.0f, 0.0f, 0.6f), vec2(1.0f, 1.0f), m_TextureStair);
		Renderer2D::DrawQuad(vec3(1.0f, 0.0f, 0.6f), vec2(1.0f, 1.0f), m_TextureBarrel);
		Renderer2D::DrawQuad(vec3(-1.5f, 0.0f, 0.6f), vec2(1.0f, 2.0f), m_TextureTree);
		Renderer2D::EndScene();

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

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Setting");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.Drawcalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("SquareColor", value_ptr(m_Color));

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBLockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(vec2*)&viewportPanelSize)
		{
			m_ViewportSize = vec2(viewportPanelSize.x, viewportPanelSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		HR_WARN("Viewport Size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		uint32_t id = m_Framebuffer->GetColorAttachmentRenderID();
		ImGui::Image((void*)id, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		
		ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}