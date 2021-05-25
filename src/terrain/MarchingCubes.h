#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Constants.h"

namespace MC
{
	struct McConfig
	{
		int subdivisions = 10;
		float scale = 1.0f;
		float surface = 0.0f;
	};

	struct GridCell
	{
		glm::vec3 point[8];
		float value[8];
	};

	void MarchCube(McConfig cfg, std::vector<float>& vertices,
		std::vector<unsigned int>& indices, const GridCell& cell);

	void MarchingCubes(McConfig cfg, std::vector<float>& vertices,
		std::vector<unsigned int>& indices, std::function<float(glm::vec3)> densityFunc)
	{
		std::vector<float> grid;
		grid.resize(cfg.subdivisions * cfg.subdivisions * cfg.subdivisions,
			cfg.surface - 1.0f);
		int subdivisions2 = cfg.subdivisions * cfg.subdivisions;
		for (int x = 1; x < cfg.subdivisions - 1; ++x)
		{
			for (int y = 1; y < cfg.subdivisions - 1; ++y)
			{
				for (int z = 1; z < cfg.subdivisions - 1; ++z)
				{
					grid[x + cfg.subdivisions * y + subdivisions2 * z] =
						densityFunc(glm::vec3(x, y, z));
				}
			}
		}

		GridCell cell;

		for (int x = 0; x < cfg.subdivisions - 1; ++x)
		{
			for (int y = 0; y < cfg.subdivisions - 1; ++y)
			{
				for (int z = 0; z < cfg.subdivisions - 1; ++z)
				{
					cell.point[0] = glm::vec3(x, y, z) * cfg.scale;
					cell.point[1] = glm::vec3(x + 1, y, z) * cfg.scale;
					cell.point[2] = glm::vec3(x + 1, y + 1, z) * cfg.scale;
					cell.point[3] = glm::vec3(x, y + 1, z) * cfg.scale;
					cell.point[4] = glm::vec3(x, y, z + 1) * cfg.scale;
					cell.point[5] = glm::vec3(x + 1, y, z + 1) * cfg.scale;
					cell.point[6] = glm::vec3(x + 1, y + 1, z + 1) * cfg.scale;
					cell.point[7] = glm::vec3(x, y + 1, z + 1) * cfg.scale;

					int baseIndex = x + cfg.subdivisions * y + subdivisions2 * z;
					cell.value[0] = grid[baseIndex];
					cell.value[1] = grid[baseIndex + 1];
					cell.value[2] = grid[baseIndex + 1 + cfg.subdivisions];
					cell.value[3] = grid[baseIndex + cfg.subdivisions];
					cell.value[4] = grid[baseIndex + subdivisions2];
					cell.value[5] = grid[baseIndex + 1 + subdivisions2];
					cell.value[6] =
						grid[baseIndex + 1 + cfg.subdivisions + subdivisions2];
					cell.value[7] = grid[baseIndex + cfg.subdivisions + subdivisions2];

					MarchCube(cfg, vertices, indices, cell);
				}
			}
		}
	}

	glm::vec3 interpolate(glm::vec3 point1, glm::vec3 point2, float value1, float value2,
		float surface)
	{
		float t = (surface - value1) / (value2 - value1);
		return point1 + t * (point2 - point1);
	}

	void vecIntoVec(std::vector<float>& vector, glm::vec3 vec)
	{
		vector.push_back(vec.x);
		vector.push_back(vec.y);
		vector.push_back(vec.z);
	}

	void MarchCube(McConfig cfg, std::vector<float>& vertices,
		std::vector<unsigned int>& indices, const GridCell& cell)
	{
		int cubeIndex = 0;

		for (int i = 0; i < 8; ++i)
		{
			if (cell.value[i] >= cfg.surface)
			{
				cubeIndex |= PowerOfTwo[i];
			}
		}

		for (int i = 0; triangleLUT[cubeIndex][i] != -1; i += 3)
		{
			int A0 = leftCornerFromEdge[triangleLUT[cubeIndex][i]];
			int B0 = rightCornerFromEdge[triangleLUT[cubeIndex][i]];

			int A1 = leftCornerFromEdge[triangleLUT[cubeIndex][i + 1]];
			int B1 = rightCornerFromEdge[triangleLUT[cubeIndex][i + 1]];

			int A2 = leftCornerFromEdge[triangleLUT[cubeIndex][i + 2]];
			int B2 = rightCornerFromEdge[triangleLUT[cubeIndex][i + 2]];

			// Vertex 1 and 2 switched for a CCW winding order
			vecIntoVec(vertices, interpolate(cell.point[A0], cell.point[B0],
									 cell.value[A0], cell.value[B0], cfg.surface));
			vecIntoVec(vertices, interpolate(cell.point[A1], cell.point[B1],
									 cell.value[A1], cell.value[B1], cfg.surface));
			vecIntoVec(vertices, interpolate(cell.point[A2], cell.point[B2],
									 cell.value[A2], cell.value[B2], cfg.surface));
			indices.push_back(indices.size());
			indices.push_back(indices.size());
			indices.push_back(indices.size());
		}
	}

}