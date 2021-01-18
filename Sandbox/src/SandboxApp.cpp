#include <Hare.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

////////////////////////////////////////
//////////////////Test//////////////////
////////////////////////////////////////
#include <glm/gtc/type_ptr.hpp>
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

		m_VertexArray = Hare::VertexArray::Create();

		float verticies[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		Hare::Ref<Hare::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hare::VertexBuffer::Create(verticies, sizeof(verticies)));

		Hare::BufferLayout layout = {
			{ Hare::ShaderDataType::Float3, "a_Position" },
			{ Hare::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indicies[3] = { 0, 1, 2 };
		Hare::Ref<Hare::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hare::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->AddIndexBuffer(indexBuffer);

		/////////////////////////////////////////////////////////////////////////////////////////

		m_SquareVA = Hare::VertexArray::Create();

		float squareVerticies[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Hare::Ref<Hare::VertexBuffer> squareVB;
		squareVB.reset(Hare::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));

		squareVB->SetLayout({ 
			{ Hare::ShaderDataType::Float3, "a_Position" },
			{ Hare::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Hare::Ref<Hare::IndexBuffer> squareIB;
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

		m_Shader.reset(Hare::Shader::Create(vertexSource, fragmentSource));

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

			uniform vec3 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_ShaderSquare.reset(Hare::Shader::Create(squareVertexSource2, squareFragmentSource2));

		// Test
		std::string textureVertexSource = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSource = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;

			uniform sampler2D u_Texture;

			in vec2 v_TexCoord;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Hare::Shader::Create(textureVertexSource, textureFragmentSource));

		m_Texture = Hare::Texture2D::Create("assets/textures/Blood.png");
		m_SecondTexture = Hare::Texture2D::Create("assets/textures/Blood1.png");

		std::dynamic_pointer_cast<Hare::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hare::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

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

		std::dynamic_pointer_cast<Hare::OpenGLShader>(m_ShaderSquare)->Bind();
		std::dynamic_pointer_cast<Hare::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat3("u_Color", m_SquareColor);

		//Hare::MaterialRef material = new Hare::Material(m_ShaderSquare);
		//Hare::MaterialInstanceRef mi = new Hare::Material(material);

		//mi->Set("u_Color", redColor);
		//mi->SetTexture("u_AlbedoMap", texture);

		//squareMesh->SetMaterial(mi);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hare::Renderer::Submit(m_ShaderSquare, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Hare::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_SecondTexture->Bind();
		Hare::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Hare::Renderer::Submit(m_Shader, m_VertexArray);

		Hare::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hare::Event& event) override
	{
	}

private:
	// --- Begin Renderer 
	Hare::Ref<Hare::Shader> m_Shader;
	Hare::Ref<Hare::VertexArray> m_VertexArray;

	Hare::Ref<Hare::Shader> m_ShaderSquare;
	Hare::Ref<Hare::VertexArray> m_SquareVA;

	Hare::Ref<Hare::Shader> m_TextureShader;
	Hare::Ref<Hare::Texture2D> m_Texture;
	Hare::Ref<Hare::Texture2D> m_SecondTexture;
	// --- End renderer

	Hare::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 10.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraAngularSpeed = 100.0f;

	glm::vec3 m_SquareColor = {0.2f, 0.3, 0.4f};
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