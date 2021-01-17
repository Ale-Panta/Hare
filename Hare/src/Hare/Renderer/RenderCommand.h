#pragma once

#include "RendererAPI.h"

namespace Hare
{
	// It just execute cmmand of RendererAPI. The RendererAPI dispatch the command based on platform.

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void DrawIndex(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndex(vertexArray); }

	private:
		static RendererAPI* s_RendererAPI;
	};
}