#include "hrpch.h"
#include "Hare/Renderer/Renderer.h"
#include "Hare/Renderer/Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hare
{
	Hare::Ref<Hare::Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(specification);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}