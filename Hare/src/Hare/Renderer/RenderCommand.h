#pragma once

#include "RendererAPI.h"

namespace Hare
{
	// It just execute cmmand of RendererAPI. The RendererAPI dispatch the command based on platform.
	// It will run in a separate thread and we need to queue RenderCommand.

	class RenderCommand
	{
	public:
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void DrawIndex(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndex(vertexArray); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }

	private:
		static RendererAPI* s_RendererAPI;
	};
}