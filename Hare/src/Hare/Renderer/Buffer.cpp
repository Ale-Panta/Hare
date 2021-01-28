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

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(verticies, size);
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

#pragma endregion

#pragma region Vertex

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indicies, size);
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

#pragma endregion
}