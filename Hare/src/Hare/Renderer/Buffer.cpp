#include "hrpch.h"
#include "Buffer.h"

#include "Renderer.h"

// #ifdef HR_PLATFORM_WINDOWS
#include "Platform/OpenGL/OpenGLBuffer.h"
// #elseif ...
// #else

namespace Hare
{
#pragma region Vertex

	VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		// #if HR_PLATFORM_WINDOWS
		case RendererAPI::None:		HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
		// #elseif ...
		case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(verticies, size);
		// #endif

		default: break;
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

#pragma endregion

#pragma region Vertex

	IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indicies, size);

		default: break;
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

#pragma endregion
}