#include "hrpch.h"
#include "Shader.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace Hare
{
	/*
	Batch renderer system.
	It allow us to render multiple geometry 
	in a single draw call.
	The way it works is the follow:
	pass all possible values to the vertex buffer 
	such as position, color, texture.
	*/

	// Single vertex properties.
	struct QuadVertex
	{
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
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

		/*
		We don't pass any data to the vertex buffer because it's marked as dynamic.
		This allow us to use batch rendering, therefore minimize the draw call.
		The buffer it's filled at the geometry creation function.
		Here we setup the layout of the buffer, the indicies.
		*/

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
			{ ShaderDataType::Float,	"a_TilingFactor" }
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
		for (size_t i = 0; i < s_Data.MaxTextureSlots; i++)	samplers[i] = i;

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
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HR_PROFILE_FUNCTION();

		// Set view projection matrix reference to all shaders.

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
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

	/*
	Draw all data passed in. As soon as some limit exceed such as texture slots or verticies,
	we flush instanlty the current data and we start a new batch. The new batch contains the 
	exceeded data.
	*/
	void Renderer2D::Flush()
	{
		// Bind textures
		for (size_t i = 0; i < s_Data.TextureSlotIndex; i++) s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndex(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.Drawcalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		// Check batch limit. If the values exceed then flush the data and start new batch.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies)
		{
			FlushAndReset();
		}

		const float textureIndex = 0.0f;	// White texture.
		const float tilingFactor = 1.0f;

		mat4 transform =
			translate(mat4(1.0f), position) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		// Set every vertex's properties of the quad...

		// Bottom-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Bottom-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawQuad(vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		// Check batch limit. If the values exceed then flush the data and start new batch.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			// We already submitted this texture?
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Find a texture index for this particular texture.
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		mat4 transform =
			translate(mat4(1.0f), position) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		// Set every vertex's properties of the quad...

		// Bottom-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Bottom-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInDegree, const vec4& color)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInDegree, color);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInDegree, const vec4& color)
	{
		HR_PROFILE_FUNCTION();

		// Check batch limit. If the values exceed then flush the data and start new batch.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies)
		{
			FlushAndReset();
		}

		const float textureIndex = 0.0f;	// White texture.
		const float tilingFactor = 1.0f;

		mat4 transform =
			translate(mat4(1.0f), position) *
			rotate(mat4(1.0f), radians(rotationInDegree), vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		// Set every vertex's properties of the quad...

		// Bottom-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Bottom-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color			= color;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size, float rotationInDegree, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		DrawRotatedQuad(vec3(position.x, position.y, 0.0f), size, rotationInDegree, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size, float rotationInDegree, const Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor)
	{
		HR_PROFILE_FUNCTION();

		// Check batch limit. If the values exceed then flush the data and start new batch.
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicies)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			// We already submitted this texture?
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Find a texture index for this particular texture.
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		mat4 transform = 
			translate(mat4(1.0f), position) * 
			rotate(mat4(1.0f), radians(rotationInDegree), vec3(0.0f, 0.0f, 1.0f)) * 
			scale(mat4(1.0f), vec3(size.x, size.y, 1.0f));

		// Set every vertex's properties of the quad...

		// Bottom-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Bottom-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-right vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Top-left vertex
		s_Data.QuadVertexBufferPtr->Position		= transform * s_Data.QuadvertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color			= tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord		= vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex		= textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor	= tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		// Number of element to draw. In a single quad there are 6 indicies.
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
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