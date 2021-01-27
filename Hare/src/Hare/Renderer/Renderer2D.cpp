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
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		HR_PROFILE_FUNCTION();

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

		s_Storage->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Storage->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Storage->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetInt("u_Texture", 0);	// Set texture slot to be 0.
	}

	void Renderer2D::ShutDown()
	{
		HR_PROFILE_FUNCTION();

		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HR_PROFILE_FUNCTION();

		// Set view projection matrix reference to all shaders.

		s_Storage->TextureShader->Bind();
		s_Storage->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		HR_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);

		s_Storage->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);
		s_Storage->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color /*= { 1.0f, 1.0f, 1.0f, 1.0f }*/)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color /*= { 1.0f, 1.0f, 1.0f, 1.0f }*/)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Storage->TextureShader->SetMat4("u_Transform", transform);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

}