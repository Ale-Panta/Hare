#include "hrpch.h"
#include "RendererAPI.h"

namespace Hare
{
	// API initialization.

	// Since for now I implement only OpenGL graphics I hardcoded it.

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
}