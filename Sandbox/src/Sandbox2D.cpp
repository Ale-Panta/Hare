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
	m_SquareVA = Hare::VertexArray::Create();

	float squareVerticies[5 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Hare::Ref<Hare::VertexBuffer> squareVB;
	squareVB.reset(Hare::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));

	squareVB->SetLayout({
		{ Hare::ShaderDataType::Float3, "a_Position" } });
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
	Hare::Ref<Hare::IndexBuffer> squareIB;
	squareIB.reset(Hare::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

	m_SquareVA->AddIndexBuffer(squareIB);

	m_FlatColorShader = Hare::Shader::Create("assets/shaders/FlatColorShader.glsl");
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

	Hare::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Hare::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Hare::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_Color);

	Hare::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hare::Renderer::EndScene();
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