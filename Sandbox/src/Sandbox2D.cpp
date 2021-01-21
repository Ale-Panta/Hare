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
	Hare::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.2f, 0.4f, 0.8f, 1.0f));
	Hare::Renderer2D::EndScene();

	// TODO: Add the following functions Shader::SetMat4, Shader::SetFlaot4
	//std::dynamic_pointer_cast<Hare::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Hare::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_Color);

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