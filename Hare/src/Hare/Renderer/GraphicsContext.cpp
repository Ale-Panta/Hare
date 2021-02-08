#include "hrpch.h"
#include "Hare/Renderer/GraphicsContext.h"
#include "Hare/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
namespace Hare
{

	Hare::Scope<Hare::GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}