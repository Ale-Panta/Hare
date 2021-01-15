#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Hare
{
	// RendererAPI interface. It must be implemented for every platform.

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}