#pragma once

#include "RenderCommand.h"

namespace Hare
{
	class Renderer
	{
	public:
		static void BeginScene(); // TODO: Add parameters in order to renderer the scene
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}