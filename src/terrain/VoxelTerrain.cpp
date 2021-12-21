#include "VoxelTerrain.h"

VoxelMesh::VoxelMesh(MarchingCubesConfig mcConfig)
{
	unsigned int size1d = mcConfig.size.x * mcConfig.size.y * mcConfig.size.z;

	m_TriangleBuffer.BufferData(size1d, nullptr);
	m_DensityBuffer.BufferData(size1d, nullptr);

	m_TriLUTBuffer.BufferData(256, triangleLUT);

	DrawArraysIndirectCommand cmd{0, 1, 0, 0};
	m_IndirectBuffer.BufferData(1, &cmd);

	m_VoxelCompute.LoadShader("shaders/MarchingCubes.comp");
	m_IndirectCounter.LoadShader("shaders/IndirectCounter.comp");

	m_Material = std::make_shared<Material>(
		Shader("shaders/MarchingCubes.vert", "shaders/MarchingCubes.frag"));
	m_Material->SetUniform("objectColor", glm::vec3(1.0f, 0.65f, 0.0f));
	m_Material->SetUniform("dirLight.direction", glm::vec3(0.5f, 1.0f, 0.2f));
	m_Material->SetUniform("dirLight.ambient", glm::vec3(0.15f, 0.15f, 0.15f));
	m_Material->SetUniform("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
}

void VoxelMesh::Regenerate(MarchingCubesConfig mcConfig)
{
	m_Scale = mcConfig.scale;

	unsigned int size1d = mcConfig.size.x * mcConfig.size.y * mcConfig.size.z;

	unsigned int counter = 0;
	m_AtomicCounter.BufferData(1, &counter);

	m_DensityBuffer.Bind();
	float *densities = (float *) glMapBuffer(m_DensityBuffer.GetBufferType(),
											 GL_WRITE_ONLY);
	for (int x = 1; x < mcConfig.size.x - 1; ++x)
	{
		for (int y = 1; y < mcConfig.size.y - 1; ++y)
		{
			for (int z = 1; z < mcConfig.size.z - 1; ++z)
			{
				densities[x + (y * mcConfig.size.x)
						  + (z * mcConfig.size.x * mcConfig.size.y)] =
					mcConfig.densityFunc(glm::vec3(x, y, z));
			}
		}
	}
	glUnmapBuffer(m_DensityBuffer.GetBufferType());

	glBindBufferBase(m_TriangleBuffer.GetBufferType(), 4, m_TriangleBuffer.GetHandle());
	glBindBufferBase(m_DensityBuffer.GetBufferType(), 5, m_DensityBuffer.GetHandle());
	glBindBufferBase(m_AtomicCounter.GetBufferType(), 6, m_AtomicCounter.GetHandle());
	glBindBufferBase(m_TriLUTBuffer.GetBufferType(), 7, m_TriLUTBuffer.GetHandle());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, m_IndirectBuffer.GetHandle());
	m_VoxelCompute.Bind();
	m_VoxelCompute.SetFloat("isoValue", mcConfig.isoValue);
	glm::uvec3 dispatchSize(mcConfig.size.x / VOXEL_COMPUTE_LOCAL_SIZE,
							mcConfig.size.y / VOXEL_COMPUTE_LOCAL_SIZE,
							mcConfig.size.z / VOXEL_COMPUTE_LOCAL_SIZE);
	glDispatchCompute(dispatchSize.x, dispatchSize.y, dispatchSize.z);

	m_IndirectCounter.Bind();
	glDispatchCompute(1, 1, 1);
}

void VoxelMesh::Draw(glm::mat4 model)
{
	model = glm::scale(model, glm::vec3(m_Scale));

	m_Material->Bind();
	m_Material->SetUniform("model", model);

	m_TriangleBuffer.Bind();
	m_IndirectBuffer.Bind();
	glDrawArraysIndirect(GL_TRIANGLES, 0);
}
