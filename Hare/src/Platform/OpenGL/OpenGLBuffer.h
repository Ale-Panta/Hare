#pragma once

#include "hare/Renderer/Buffer.h"

namespace Hare
{
#pragma region Vertex

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		inline virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

#pragma endregion

#pragma region Index

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
		virtual ~OpenGLIndexBuffer();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		inline virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_Count;
		uint32_t m_RendererID;
	};

#pragma endregion
}