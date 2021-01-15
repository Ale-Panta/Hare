#include "hrpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hare
{
	// API initialization.

	// Since for now I implement only OpeGL graphics I hardcoded it.

	// TODO: make API swappable.

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}