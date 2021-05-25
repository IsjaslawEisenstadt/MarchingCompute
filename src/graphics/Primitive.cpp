#include "Primitive.h"

Primitive::Primitive(const std::vector<float>& vertices, const VertexBufferLayout& layout,
	std::shared_ptr<Material> material)
	: vertexBuffer(vertices), vertexArray(), material(material)
{
	vertexArray.AddBuffer(vertexBuffer, layout);
}

Primitive::Primitive(const std::vector<float>& vertices,
	const std::vector<unsigned int>& indices, const VertexBufferLayout& layout,
	std::shared_ptr<Material> material)
	: vertexBuffer(static_cast<unsigned int>(vertices.size()), vertices.data()),
	  indexBuffer(std::make_optional<IndexBuffer>(
		  static_cast<unsigned int>(indices.size()), indices.data())),
	  vertexArray(), material(material)
{
	vertexArray.AddBuffer(vertexBuffer, layout);
}

void Primitive::Bind() const
{
	vertexArray.Bind();
	if (indexBuffer)
		indexBuffer->Bind();
}

void Primitive::Draw(glm::mat4 model) const
{
	material->SetUniform("model", model);
	material->Bind();
	Bind();
	if (indexBuffer)
	{
		glDrawElements(mode, indexBuffer->GetCount(), indexBuffer->GetComponentType(), nullptr);
	}
	else
	{
		glDrawArrays(mode, 0, vertexBuffer.GetCount());
	}
}
