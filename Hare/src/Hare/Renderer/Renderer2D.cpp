#include "hrpch.h"
#include "Shader.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

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

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Storage->WhiteTexture->Bind();

		mat4 transform = 
			translate(mat4(1.0f), position) * 
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Storage->TextureShader->SetMat4("u_Transform", transform);
		s_Storage->QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", tintColor);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		mat4 transform = 
			translate(mat4(1.0f), position) * 
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Storage->TextureShader->SetMat4("u_Transform", transform);
		s_Storage->QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInRad, const vec4& color)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInRad, color);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInRad, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Storage->TextureShader->SetFloat4("u_Color", color);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Storage->WhiteTexture->Bind();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), rotationInRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Storage->TextureShader->SetMat4("u_Transform", transform);
		s_Storage->QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInRad, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		s_Storage->TextureShader->SetFloat4("u_Color", tintColor);
		s_Storage->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), rotationInRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Storage->TextureShader->SetMat4("u_Transform", transform);
		s_Storage->QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Storage->QuadVertexArray);
	}
}