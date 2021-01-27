#include "hrpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

using namespace std;

namespace Hare
{
	Ref<Texture2D> Texture2D::Create(const string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(filepath);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Hare::Ref<Hare::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}