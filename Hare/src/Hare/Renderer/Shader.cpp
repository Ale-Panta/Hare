#include "hrpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hare
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSource, fragmentSource);
			// #endif

		default: break;
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}