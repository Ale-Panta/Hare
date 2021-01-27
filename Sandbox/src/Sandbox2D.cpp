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

	// Renderer
	{
		HR_PROFILE_SCOPE("Render Preparation");

		Hare::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hare::RenderCommand::Clear();
	}
	{
		HR_PROFILE_SCOPE("Render Draw");

		Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hare::Renderer2D::DrawRotatedQuad(vec3(-1.0f, 0.0f, 0.0f), vec2(0.8f, 0.8f), radians(30.f), m_Color);
		Hare::Renderer2D::DrawQuad(vec3(0.5f, -0.5f, 0.0f), vec2(0.5f, 0.75f), vec4(0.7f, 0.4f, 0.5f, 1.0f));
		Hare::Renderer2D::DrawRotatedQuad(vec3(0.0f, 0.0f, -0.1f), vec2(10.0f, 10.0f), radians(45.0f), m_Texture, 10.0f, vec4(0.3f, 0.2f, 0.5f, 1.0f));
		Hare::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HR_PROFILE_FUNCTION();

	ImGui::Begin("Setting");
	ImGui::ColorEdit4("SquareColor", value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hare::Event& e)
{
	m_CameraController.OnEvent(e);
}