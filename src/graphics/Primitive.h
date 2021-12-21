#pragma once

#include <iostream>
#include <optional>

#include "Buffer.h"
#include "Material.h"

class Primitive
{
	VertexBuffer m_VertexBuffer;
	std::optional<IndexBuffer> m_IndexBuffer;
	const VertexArray m_VertexArray;
	std::shared_ptr<Material> m_Material;

	unsigned int mode = GL_TRIANGLES;

public:
	Primitive();

	[[maybe_unused]] Primitive(const std::vector<float> &vertices,
							   const VertexBufferLayout &layout,
							   std::shared_ptr<Material> material);

	Primitive(const std::vector<float> &vertices,
			  const std::vector<unsigned int> &indices,
			  const VertexBufferLayout &layout,
			  std::shared_ptr<Material> material);

	void Bind() const;
	void Draw(glm::mat4 model = glm::mat4(1.0f));

	void SetVertices(const std::vector<float> &vertices,
					 const VertexBufferLayout &layout);
	void SetIndices(const std::vector<unsigned int> &indices);

	inline unsigned int GetMode() const { return mode; }
	[[maybe_unused]] inline void SetMode(unsigned int newMode) { mode = newMode; }

	inline void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }
	inline std::shared_ptr<Material> GetMaterial() const { return m_Material; }
};

/*
struct Mesh
{
	std::vector<Primitive> primitives;

	void Draw(glm::mat4 transform) const
	{
		for (const auto& primitive : primitives)
		{
			primitive.Draw(transform);
		}
	}
};

*/
/*

class Node
{
	glm::mat4 transform;
	std::optional<std::shared_ptr<Mesh>> mesh;
	std::vector<Node> children;

public:
	explicit Node(glm::mat4 transform = glm::mat4(1.0f)) : transform(transform) {}

	explicit Node(std::shared_ptr<Mesh> mesh, glm::mat4 transform = glm::mat4(1.0f))
		: transform(transform), mesh(mesh)
	{
	}

	void AddChild(const Node& node) { children.push_back(node); }
	void DrawTree(glm::mat4 root = glm::mat4(1.0f)) const
	{
		root *= transform;
		if (mesh)
		{
			// a rare double dereference
			(*mesh)->Draw(root);
		}
		for (const auto& child : children)
		{
			child.DrawTree(root);
		}
	}
};
*/
