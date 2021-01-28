#pragma once

namespace Hare
{
	/*
	This file contains both vertex, index buffer and buffer layout just for the sake 
	of readability.

	Those class below represent the interfices that will be implemented for each
	API such as DirectX and OpenGL. Since it's an interface they do not contain any
	variables. Specific data are contained inside the API specification.
	To retrieve some specific data you need to declare a virtual function that
	return that type of data.
	*/

#pragma region Layout

	enum class ShaderDataType : uint8_t
	{
		None	= 0,
		Bool,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
	};

	// Return the size of the variable type in bytes.
	static uint32_t SharedDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Hare::ShaderDataType::Bool:	return 1;
		case Hare::ShaderDataType::Float:	return 4;
		case Hare::ShaderDataType::Float2:	return 4 * 2;
		case Hare::ShaderDataType::Float3:	return 4 * 3;
		case Hare::ShaderDataType::Float4:	return 4 * 4;
		case Hare::ShaderDataType::Mat3:	return 4 * 3 * 3;
		case Hare::ShaderDataType::Mat4:	return 4 * 4 * 4;
		case Hare::ShaderDataType::Int:		return 4;
		case Hare::ShaderDataType::Int2:	return 4 * 2;
		case Hare::ShaderDataType::Int3:	return 4 * 3;
		case Hare::ShaderDataType::Int4:	return 4 * 4;
		}

		HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	// Group up vertex buffer element properties.
	struct BufferElement
	{
		// Properties are initialized when the struct will be constructed.
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		UINT_PTR Offset;	// UINT_PTR avoid error C4312. We can also restore the type to uint32_t and in the double cast it when error occur.
		bool Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(SharedDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		// Return the count of of element in variable type.
		inline uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Hare::ShaderDataType::Bool:	return 1;
			case Hare::ShaderDataType::Float:	return 1;
			case Hare::ShaderDataType::Float2:	return 2;
			case Hare::ShaderDataType::Float3:	return 3;
			case Hare::ShaderDataType::Float4:	return 4;
			case Hare::ShaderDataType::Mat3:	return 3 * 3;
			case Hare::ShaderDataType::Mat4:	return 4 * 4;
			case Hare::ShaderDataType::Int:		return 1;
			case Hare::ShaderDataType::Int2:	return 2;
			case Hare::ShaderDataType::Int3:	return 3;
			case Hare::ShaderDataType::Int4:	return 4;
			}

			HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	// Tight together Vertex buffer with a layout.
	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

	public:
		inline const uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement> GetElements() const { return m_Elements; }

		// --- Iterators -----------------------------------------------------------------------
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
		// --------------------------------------------------------------------------------------

	private:
		inline void CalculateOffsetAndStride()
		{
			m_Stride = 0;
			uint32_t offset = 0;

			for (auto& elements : m_Elements)
			{
				elements.Offset = offset;
				offset += elements.Size;
				m_Stride += elements.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

#pragma endregion

#pragma region Vertex buffer

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() { }

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* verticies, uint32_t size);
	};

#pragma endregion

#pragma region Index buffer

	/*
	Currently Hare only supports 32-bit index buffer.
	*/

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indicies, uint32_t count);
	};

#pragma endregion
}