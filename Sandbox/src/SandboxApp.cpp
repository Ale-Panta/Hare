#include <Hare.h>

#include "imgui/imgui.h"


////////////////////////////////////////
//////////////////Test//////////////////
////////////////////////////////////////
#include <glm/gtc/matrix_transform.hpp>

/*
This is just an example of how to create new custom layers.
*/
class ExampleLayer : public Hare::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(glm::vec3(0.0f))
	{

		m_VertexArray.reset(Hare::VertexArray::Create());

		float verticies[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		m_VertexBuffer.reset(Hare::VertexBuffer::Create(verticies, sizeof(verticies)));

		Hare::BufferLayout layout = {
			{ Hare::ShaderDataType::Float3, "a_Position" },
			{ Hare::ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indicies[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Hare::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->AddIndexBuffer(m_IndexBuffer);

		/////////////////////////////////////////////////////////////////////////////////////////

		m_SquareVA.reset(Hare::VertexArray::Create());

		float squareVerticies[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Hare::VertexBuffer> squareVB;
		squareVB.reset(Hare::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));

		squareVB->SetLayout({ { Hare::ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hare::IndexBuffer> squareIB;
		squareIB.reset(Hare::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		m_SquareVA->AddIndexBuffer(squareIB);

		// Test
		std::string vertexSource = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position, 1.0) * 0.5 + 0.5 + vec4(1.0, 0.4, 0.1, 1.0);
			}
		)";

		m_Shader.reset(new Hare::Shader(vertexSource, fragmentSource));

		// Test
		std::string squareVertexSource2 = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSource2 = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.4, 0, 1, 1);
			}
		)";

		m_ShaderSquare.reset(new Hare::Shader(squareVertexSource2, squareFragmentSource2));
	}

	void OnUpdate(Hare::TimeStep ts) override 
	{
		if (Hare::Input::IsKeyPressed(HR_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMovementSpeed * ts;

		else if (Hare::Input::IsKeyPressed(HR_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMovementSpeed * ts;

		if (Hare::Input::IsKeyPressed(HR_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMovementSpeed * ts;

		else if (Hare::Input::IsKeyPressed(HR_KEY_UP))
			m_CameraPosition.y += m_CameraMovementSpeed * ts;

		if (Hare::Input::IsKeyPressed(HR_KEY_A))
			m_CameraRotation += m_CameraAngularSpeed * ts;

		else if (Hare::Input::IsKeyPressed(HR_KEY_D))
			m_CameraRotation -= m_CameraAngularSpeed * ts;

		Hare::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hare::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hare::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hare::Renderer::Submit(m_ShaderSquare, m_SquareVA, transform);
			}
		}

		Hare::Renderer::Submit(m_Shader, m_VertexArray);

		Hare::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Hare::Event& event) override
	{
	}

private:
	// --- Begin Renderer 
	std::shared_ptr<Hare::Shader> m_Shader;
	std::shared_ptr<Hare::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hare::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Hare::VertexArray> m_VertexArray;

	std::shared_ptr<Hare::Shader> m_ShaderSquare;
	std::shared_ptr<Hare::VertexArray> m_SquareVA;
	// --- End renderer

	Hare::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 10.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraAngularSpeed = 100.0f;
};

////////////////////////////////////////
/////////Client Application/////////////
////////////////////////////////////////

class Sandbox : public Hare::Application 
{
public:
	Sandbox()
	{
		// Uncomment the line below to see how react the custom layer in console.
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

/// <summary>
/// Definition of Hare::CreateApplication.
/// </summary>
/// <returns>Hare::Application*</returns>
/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
/// <see cref="$(SolutionDir)Hare/src/Hare/Application.h"/>
Hare::Application* Hare::CreateApplication()
{
	return new Sandbox();
}