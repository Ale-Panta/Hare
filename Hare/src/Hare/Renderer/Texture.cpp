#include "hrpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hare
{
	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(filepath);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}