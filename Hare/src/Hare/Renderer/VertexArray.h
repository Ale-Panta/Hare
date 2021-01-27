#pragma once

#include "Hare/Renderer/Buffer.h"

#include <memory>

namespace Hare
{
	// Vertex Array interfaces. It must be implemented for every platform.

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// It must be called after the layout is set.
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}