#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1200.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Hare::Texture2D::Create("assets/textures/ADC2021.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hare::TimeStep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Renderer
	Hare::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hare::RenderCommand::Clear();

	Hare::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hare::Renderer2D::DrawQuad(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.8f, 0.8f), glm::vec4(0.2f, 0.4f, 0.8f, 1.0f));
	Hare::Renderer2D::DrawQuad(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.5f, 0.75f), glm::vec4(0.7f, 0.4f, 0.5f, 1.0f));
	Hare::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f, 10.0f), m_Texture, glm::vec4(0.7f, 0.4f, 0.5f, 1.0f));
	Hare::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hare::Event& e)
{
	m_CameraController.OnEvent(e);
}