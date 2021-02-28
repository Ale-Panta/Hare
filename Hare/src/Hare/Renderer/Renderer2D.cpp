#include "hrpch.h"
#include "Hare/Renderer/Shader.h"
#include "Hare/Renderer/Renderer2D.h"
#include "Hare/Renderer/VertexArray.h"
#include "Hare/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace Hare
{
	// Batch renderer system.
	// It allow us to render multiple geometry 
	// in a single draw call.
	// The way it works is the follow:
	// pass all possible values to the vertex buffer 
	// such as position, color, texture.

	// Single vertex properties.
	struct QuadVertex
	{
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor - Only
		int EntityID;
	};

	// Rendering struct of the game. It depends on the game.
	struct Renderer2DData
	{
		// Maximum numbers for draw call.
		static const uint32_t MaxQuads			= 10000;
		static const uint32_t MaxVerticies		= MaxQuads * 4;
		static const uint32_t MaxIndicies		= MaxQuads * 6;
		static const uint32_t MaxTextureSlots	= 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		/// Number of element to draw. In a single quad there are 6 indicies.
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase	= nullptr;
		QuadVertex* QuadVertexBufferPtr		= nullptr;

		array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;	// Slot index 0 --> white texture

		vec4 QuadvertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		HR_PROFILE_FUNCTION();

		// We don't pass any data to the vertex buffer because it's marked as dynamic.
		// This allow us to use batch rendering, therefore minimize the draw call.
		// The buffer it's filled at the geometry creation function.
		// Here we setup the layout of the buffer, the indicies.

		// Create vertex array based on the graphics API we are using.
		s_Data.QuadVertexArray	= VertexArray::Create();

		// Create vertex buffer based on the graphics API we are using.
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVerticies * sizeof(QuadVertex));

		// Set the layout of the buffer. The data type must be in the proper order.
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position" },
			{ ShaderDataType::Float4,	"a_Color" },
			{ ShaderDataType::Float2,	"a_TexCoord" },
			{ ShaderDataType::Float,	"a_TexIndex" },
			{ ShaderDataType::Float,	"a_TilingFactor" },
			{ ShaderDataType::Int,		"a_EntityID" }
		});

		// Add vertex buffer to the list o vertex buffer.
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticies];

		///////////////////////////////////////////////////////////////////////////////////////
		// Begin quad indicies ////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		uint32_t* quadIndicies = new uint32_t[s_Data.MaxIndicies];

		// Vertex offset
		uint32_t offset = 0;

		for (uint32_t i = 0; i < s_Data.MaxIndicies; i += 6)	// Must be multiple of 6.
		{
			quadIndicies[i + 0] = offset + 0;
			quadIndicies[i + 1] = offset + 1;
			quadIndicies[i + 2] = offset + 2;

			quadIndicies[i + 3] = offset + 2;
			quadIndicies[i + 4] = offset + 3;
			quadIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		// Create index buffer based on the graphics API we are using.
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndicies, s_Data.MaxIndicies);

		// Set index buffer.
		s_Data.QuadVertexArray->AddIndexBuffer(quadIB);

		delete[] quadIndicies;

		///////////////////////////////////////////////////////////////////////////////////////
		// End quad indicies //////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		// Create texture based on the graphics API we are using.
		s_Data.WhiteTexture = Texture2D::Create(1, 1);

		// Set it as white
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Set samplers
		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++) samplers[i] = i;

		// Create shader based on the graphics API we are using.
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();

		// Set texture slot to be 0.
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set all texture slots to zero.
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadvertexPositions[0] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadvertexPositions[1] = vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadvertexPositions[2] = vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadvertexPositions[3] = vec4(-0.5f,  0.5f, 0.0f, 1.0f);
	}

	void Renderer2D::ShutDown()
	{
		HR_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HR_PROFILE_FUNCTION();

		// Set view projection matrix reference to all shaders.

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}


	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		HR_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}


	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		HR_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		HR_PROFILE_FUNCTION();

		Flush();
	}

	// Draw all data passed in. As soon as some limit exceed such as texture slots or verticies,
	// we flush instanlty the current data and we start a new batch. The new batch contains the 
	// exceeded data.
	void Renderer2D::Flush()
	{
		// Nothing to draw.
		if (s_Data.QuadIndexCount == 0) return;

		// Retrieve how many elements there are.
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);

		// Setup the data.
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndex(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.Drawcalls++;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transfrom, const glm::vec4& color, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		const float tilingFactor = 1.0f;

		// Check batch rendering boundaries.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies) NextBatch();

		// Set every vertex's properties of the quad...

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transfrom * s_Data.QuadvertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transfrom, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr vec2 textureCoords[] = { 
			{0.0f, 0.0f}, 
			{1.0f, 0.0f}, 
			{1.0f, 1.0f}, 
			{0.0f, 1.0f} 
		};

		// Check batch rendering boundaries.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies) NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			// We already submitted this texture?
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Set every vertex's properties of the quad...

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transfrom * s_Data.QuadvertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transfrom, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		const vec2* textureCoords = subTexture->GetTexCoord();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		// Check batch rendering boundaries.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies) NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			// We already submitted this texture?
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Set every vertex's properties of the quad...

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transfrom * s_Data.QuadvertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float angleRad, const vec4& color)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, angleRad, color);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float angleRad, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), angleRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float angleRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, angleRad, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float angleRad, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), angleRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angleRad, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, angleRad, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angleRad, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		HR_PROFILE_FUNCTION();

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), angleRad, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}


	void Renderer2D::DrawSprite(const glm::mat4& transfrom, SpriteRendererComponent& spriteRendererComponent, int entityID)
	{
		if (spriteRendererComponent.ToggleTexture)
			DrawQuad(transfrom, spriteRendererComponent.Texture, spriteRendererComponent.TilingFactor, spriteRendererComponent.Color, entityID);
		else
			DrawQuad(transfrom, spriteRendererComponent.Color, entityID);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}