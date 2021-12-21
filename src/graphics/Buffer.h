#pragma once

#include <cassert>
#include <vector>

#include <glad/glad.h>

template<unsigned int BufferType, typename ComponentType, unsigned Usage = GL_STATIC_DRAW>
class Buffer
{
	unsigned int m_Handle;
	unsigned int m_Count;

public:
	Buffer() { glGenBuffers(1, &m_Handle); }

	Buffer(unsigned int count, const void *data) : Buffer() { BufferData(count, data); }

	explicit Buffer(std::vector<ComponentType> data) : Buffer()
	{
		BufferData(data.size(), data.data());
	}

	void BufferData(unsigned int count, const void *data)
	{
		m_Count = count;
		Bind();
		glBufferData(BufferType, m_Count * sizeof(ComponentType), data, Usage);
	}

	~Buffer() { DestroyBuffer(); }

	inline void Bind() const { glBindBuffer(BufferType, m_Handle); }

	inline void DestroyBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
		m_Handle = 0;
	}

	inline unsigned int GetCount() const { return m_Count; }
	inline unsigned int GetBufferType() const { return BufferType; }
	inline unsigned int GetComponentType() const
	{
		return GetComponentType<ComponentType>();
	}
	inline unsigned int GetHandle() const { return m_Handle; }

private:
	template<typename T>
	unsigned int GetComponentType() const;

	template<>
	unsigned int GetComponentType<float>() const
	{
		return GL_FLOAT;
	}

	template<>
	unsigned int GetComponentType<unsigned int>() const
	{
		return GL_UNSIGNED_INT;
	}

	template<>
	unsigned int GetComponentType<unsigned char>() const
	{
		return GL_UNSIGNED_BYTE;
	}
};

using VertexBuffer = Buffer<GL_ARRAY_BUFFER, float>;
using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int>;

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 4;
			default: assert(false); return 0;
		};
	}
};

class VertexBufferLayout
{
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0){};

	template<typename T>
	VertexBufferLayout &Push(unsigned int count);

	template<>
	VertexBufferLayout &Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
		return *this;
	}

	template<>
	VertexBufferLayout &Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		return *this;
	}

	template<>
	VertexBufferLayout &Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		return *this;
	}

	[[nodiscard]] inline const std::vector<VertexBufferElement> &GetElements() const
	{
		return m_Elements;
	}

	[[nodiscard]] inline unsigned int GetStride() const { return m_Stride; }
};

class VertexArray
{
	unsigned int m_Handle;

public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const;
};
