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

	m_Texture = Hare::Texture2D::Create("assets/textures/Blood.png");
}

void Sandbox2D::OnDetach()
{
	HR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hare::TimeStep ts)
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
	{
		static float rotation = 0.0f;
		rotation += ts * 20;

		HR_PROFILE_SCOPE("Render Draw");

		Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
		// Hare::Renderer2D::DrawRotatedQuad(vec3(-1.0f, 0.0f, 0.0f), vec2(0.8f, 0.8f), radians(30.f), m_Color);
		Hare::Renderer2D::DrawQuad(vec2(0.0f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.4f, 0.5f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec2(0.5f, -0.5f), vec2(0.5f, 0.75f), vec4(0.7f, 0.1f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec3(0.0f), vec2(10.0f, 10.0f), m_Texture, 5.0f, vec4(0.3f, 0.2f, 0.5f, 1.0f));
		Hare::Renderer2D::DrawQuad(vec3(0.0f, 0.0f, 0.1f), vec2(5.0f, 5.0f), m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawRotatedQuad(vec3(-1.0f, -1.0f, 0.2f), vec2(2.0f, 2.0f), 37.0f, m_Texture, 10.0f, vec4(0.8f, 0.8f, 0.8f, 1.0f));
		Hare::Renderer2D::DrawRotatedQuad(vec3(1.0f, 0.0f, 0.3f), vec2(2.0f, 2.0f), rotation, vec4(0.8f, 0.8f, 0.8f, 1.0f));
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