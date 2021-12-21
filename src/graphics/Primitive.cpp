#include "Primitive.h"

Primitive::Primitive() {}

Primitive::Primitive(const std::vector<float> &vertices,
					 const VertexBufferLayout &layout,
					 std::shared_ptr<Material> material)
	: m_Material(material)
{
	SetVertices(vertices, layout);
}

Primitive::Primitive(const std::vector<float> &vertices,
					 const std::vector<unsigned int> &indices,
					 const VertexBufferLayout &layout,
					 std::shared_ptr<Material> material)
	: m_Material(material)
{
	SetVertices(vertices, layout);
	SetIndices(indices);
}

void Primitive::Bind() const
{
	m_VertexArray.Bind();
	if (m_IndexBuffer)
		m_IndexBuffer->Bind();
}

void Primitive::Draw(glm::mat4 model)
{
	m_Material->SetUniform("model", model);
	m_Material->Bind();
	Bind();
	if (m_IndexBuffer)
	{
		glDrawElements(mode,
					   m_IndexBuffer->GetCount(),
					   m_IndexBuffer->GetComponentType(),
					   nullptr);
	}
	else
	{
		glDrawArrays(mode, 0, m_VertexBuffer.GetCount());
	}
}

void Primitive::SetVertices(const std::vector<float> &vertices,
							const VertexBufferLayout &layout)
{
	m_VertexBuffer.BufferData(static_cast<unsigned int>(vertices.size()),
							  vertices.data());
	m_VertexArray.AddBuffer(m_VertexBuffer, layout);
}

void Primitive::SetIndices(const std::vector<unsigned int> &indices)
{
	m_IndexBuffer = IndexBuffer(static_cast<unsigned int>(indices.size()),
								indices.data());
}
