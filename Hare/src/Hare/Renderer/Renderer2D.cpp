#include "hrpch.h"
#include "Renderer2D.h"

#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hare
{
	// Rendering struct of the game. It depends on the game.
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		s_Storage = new Renderer2DStorage();

		s_Storage->QuadVertexArray = VertexArray::Create();

		float squareVerticies[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" } 
		});
		s_Storage->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		s_Storage->QuadVertexArray->AddIndexBuffer(squareIB);

		s_Storage->FlatColorShader =	Shader::Create("assets/shaders/FlatColorShader.glsl");
		s_Storage->TextureShader =		Shader::Create("assets/shaders/Texture.glsl");
		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetInt("u_Texture", 0);	// Set texture slot to be 0.
	}

	void Renderer2D::ShutDown()
	{
		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Set view projection matrix reference to all shaders.

		s_Storage->FlatColorShader->Bind();
		s_Storage->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Storage->FlatColorShader->Bind();
		s_Storage->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation*/ glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->FlatColorShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation*/ glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

}