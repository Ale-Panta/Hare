#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1200.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HR_PROFILE_FUNCTION();

	m_SpreadSheet = Hare::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
	m_TextureStair = Hare::SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(7, 6), vec2(128.0f, 128.0f));
	m_TextureBarrel = Hare::SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(8, 2), vec2(128.0f, 128.0f));
	m_TextureTree = Hare::SubTexture2D::CreateFromCoords(m_SpreadSheet, vec2(2, 1), vec2(128.0f, 128.0f), vec2(1, 2));

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

void Sandbox2D::OnDetach()
{
	HR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hare::DeltaTime dt)
{
	HR_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Reset stats here.
	Hare::Renderer2D::ResetStats();

	// Renderer
	{
		HR_PROFILE_SCOPE("Render Preparation");

		Hare::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hare::RenderCommand::Clear();
	}

#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 20;

		HR_PROFILE_SCOPE("Render Draw");

		Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hare::Renderer2D::DrawQuad(vec2(0.0f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.4f, 0.5f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec2(0.5f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.1f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec3(0.0f), vec2(10.0f, 10.0f), m_Texture, 5.0f, vec4(0.3f, 0.2f, 0.5f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec3(0.0f, 0.0f, 0.1f), vec2(5.0f, 5.0f), m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawRotatedQuad(vec3(-1.0f, -1.0f, 0.2f), vec2(2.0f, 2.0f), radians(37.0f), m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawRotatedQuad(vec3( 1.0f,  0.0f, 0.3f), vec2(2.0f, 2.0f), radians(rotation), vec4(0.8f, 0.8f, 0.8f, 1.0f));
		Hare::Renderer2D::EndScene();

		Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
				Hare::Renderer2D::DrawQuad(vec3(x, y, 0.1f), vec2(0.45f, 0.45f), color);
			}
		}
		Hare::Renderer2D::EndScene();
	}
#endif

#if PARTICLE
	if (Hare::Input::IsMouseButtonPressed(HR_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hare::Input::GetMousePosition();
		auto width = Hare::Application::Get().GetWindow().GetWidth();
		auto height = Hare::Application::Get().GetWindow().GetHeight();

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

	Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hare::Renderer2D::DrawQuad(vec3(0.0f, 0.0f, 0.6f), vec2(1.0f, 1.0f), m_TextureStair);
	Hare::Renderer2D::DrawQuad(vec3(1.0f, 0.0f, 0.6f), vec2(1.0f, 1.0f), m_TextureBarrel);
	Hare::Renderer2D::DrawQuad(vec3(-1.5f, 0.0f, 0.6f), vec2(1.0f, 2.0f), m_TextureTree);
	Hare::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	HR_PROFILE_FUNCTION();

	ImGui::Begin("Setting");

	auto stats = Hare::Renderer2D::GetStats();
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw calls: %d", stats.Drawcalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("SquareColor", value_ptr(m_Color));

	ImGui::End();
}

void Sandbox2D::OnEvent(Hare::Event& e)
{
	m_CameraController.OnEvent(e);
}