#pragma once

#include <functional>
#include "game/FirstPersonGame.h"

#define VOXEL_COMPUTE_LOCAL_SIZE 10

struct MarchingCubesConfig
{
	using DensityFunc = std::function<float(glm::vec3)>;

	glm::ivec3 size{100};
	float scale = 1.0f;
	float isoValue = 0.0f;
	DensityFunc densityFunc;
};

class VoxelMesh
{
	struct Triangle
	{
		glm::vec4 points[3];
		glm::vec4 normals[3];
	};

	struct DrawArraysIndirectCommand
	{
		unsigned int count;
		unsigned int instanceCount;
		unsigned int first;
		unsigned int baseInstance;
	};

	// TODO: use glBufferStorage for SSBOs (maybe by inheriting from Buffer?)
	using TriangleBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, Triangle, GL_DYNAMIC_COPY>;
	using FloatBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, float, GL_DYNAMIC_DRAW>;
	using AtomicCounter = Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned, GL_DYNAMIC_DRAW>;
	using TriLUTBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, int[16], GL_STATIC_DRAW>;
	using IndirectBuffer =
		Buffer<GL_DRAW_INDIRECT_BUFFER, DrawArraysIndirectCommand, GL_DYNAMIC_COPY>;

	TriangleBuffer m_TriangleBuffer;
	AtomicCounter m_AtomicCounter;
	FloatBuffer m_DensityBuffer;
	TriLUTBuffer m_TriLUTBuffer;
	IndirectBuffer m_IndirectBuffer;

	Shader m_VoxelCompute;
	Shader m_IndirectCounter;
	std::shared_ptr<Material> m_Material;

	float m_Scale;

public:
	VoxelMesh(MarchingCubesConfig mcConfig);

	void Regenerate(MarchingCubesConfig mcConfig);

	void Draw(glm::mat4 model);

	inline std::shared_ptr<Material> GetMaterial() const { return m_Material; }
};
