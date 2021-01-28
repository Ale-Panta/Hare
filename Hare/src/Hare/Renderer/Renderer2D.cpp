#include "hrpch.h"
#include "Shader.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Hare
{
	struct QuadVertex
	{
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;
		// TODO: color, texid.
	};


	// Rendering struct of the game. It depends on the game.
	struct Renderer2DData
	{
		// Maximum numbers for draw call.
		const uint32_t MaxQuads		= 10000;
		const uint32_t MaxVerticies = MaxQuads * 4;
		const uint32_t MaxIndicies	= MaxQuads * 6;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> QuadVertexArray;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase	= nullptr;
		QuadVertex* QuadVertexBufferPtr		= nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		HR_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVerticies * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticies];

		uint32_t* quadIndicies = new uint32_t[s_Data.MaxIndicies];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicies; i += 6)
		{
			quadIndicies[i + 0] = offset + 0;
			quadIndicies[i + 1] = offset + 1;
			quadIndicies[i + 2] = offset + 2;

			quadIndicies[i + 3] = offset + 2;
			quadIndicies[i + 4] = offset + 3;
			quadIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndicies, s_Data.MaxIndicies);
		s_Data.QuadVertexArray->AddIndexBuffer(quadIB);
		delete[] quadIndicies;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);	// Set texture slot to be 0.
	}

	void Renderer2D::ShutDown()
	{
		HR_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HR_PROFILE_FUNCTION();

		// Set view projection matrix reference to all shaders.

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		HR_PROFILE_FUNCTION();

		// Retrieve how many elements there are.
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;

		// Setup the data.
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}


	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndex(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		// set every vertex's properties of the quad.
		s_Data.QuadVertexBufferPtr->Position	= position;
		s_Data.QuadVertexBufferPtr->Color		= color;
		s_Data.QuadVertexBufferPtr->TexCoord	= vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position	= vec3(position.x + size.x, position.y, 0.0f);
		s_Data.QuadVertexBufferPtr->Color		= color;
		s_Data.QuadVertexBufferPtr->TexCoord	= vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position	= vec3(position.x + size.x, position.y + size.y, 0.0f);
		s_Data.QuadVertexBufferPtr->Color		= color;
		s_Data.QuadVertexBufferPtr->TexCoord	= vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position	= vec3(position.x, position.y + size.y, 0.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		//s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//s_Data.WhiteTexture->Bind();

		//mat4 transform = 
		//	translate(mat4(1.0f), position) * 
		//	scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		//s_Data.TextureShader->SetMat4("u_Transform", transform);
		//s_Data.QuadVertexArray->Bind();

		//RenderCommand::DrawIndex(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		mat4 transform = 
			translate(mat4(1.0f), position) * 
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInRad, const vec4& color)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInRad, color);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInRad, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), rotationInRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInRad, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), rotationInRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVertexArray->Bind();

		RenderCommand::DrawIndex(s_Data.QuadVertexArray);
	}
}