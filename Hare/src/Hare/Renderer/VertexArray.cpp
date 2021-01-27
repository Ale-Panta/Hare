#include "hrpch.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

using namespace std;

namespace Hare
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return make_shared<OpenGLVertexArray>();
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}