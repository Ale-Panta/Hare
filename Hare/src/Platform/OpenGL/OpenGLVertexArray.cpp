#include "hrpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hare
{
	// Temp
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hare::ShaderDataType::Bool:	return GL_BOOL;
		case Hare::ShaderDataType::Float:	return GL_FLOAT;
		case Hare::ShaderDataType::Float2:	return GL_FLOAT;
		case Hare::ShaderDataType::Float3:	return GL_FLOAT;
		case Hare::ShaderDataType::Float4:	return GL_FLOAT;
		case Hare::ShaderDataType::Mat3:	return GL_FLOAT;
		case Hare::ShaderDataType::Mat4:	return GL_FLOAT;
		case Hare::ShaderDataType::Int:		return GL_INT;
		case Hare::ShaderDataType::Int2:	return GL_INT;
		case Hare::ShaderDataType::Int3:	return GL_INT;
		case Hare::ShaderDataType::Int4:	return GL_INT;
		}

		HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer as no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}