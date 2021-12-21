#include "Buffer.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &m_Handle); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_Handle); }

void VertexArray::Bind() const { glBindVertexArray(m_Handle); }

void VertexArray::AddBuffer(const VertexBuffer &vb,
							const VertexBufferLayout &layout) const
{
	Bind();
	vb.Bind();
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;

	for (size_t i = 0; i < elements.size(); ++i)
	{
		const auto &element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,
							  element.count,
							  element.type,
							  element.normalized,
							  layout.GetStride(),
							  (const void *) offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
