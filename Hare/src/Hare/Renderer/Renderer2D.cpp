#include "hrpch.h"
#include "Renderer2D.h"

#include "Shader.h"
#include "VertexArray.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hare
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		s_Storage = new Renderer2DStorage();

		s_Storage->QuadVertexArray = VertexArray::Create();

		float squareVerticies[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" } 
		});
		s_Storage->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		s_Storage->QuadVertexArray->AddIndexBuffer(squareIB);

		s_Storage->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Storage->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Storage->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Storage->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Storage->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Storage->FlatColorShader)->UploadUniformFloat4("u_Color", color);
		
		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
	}
}