#include <fast_noise_lite/FastNoiseLite.h>
#include <imgui/imgui.h>
#include "game/FirstPersonGame.h"
#include "terrain/MarchingCubes.h"
#include "core/FileSystem.h"

struct NoiseConfig
{
	int type = FastNoiseLite::NoiseType_OpenSimplex2;
	int seed = 1337;
	int octaves = 3;
	float gain = 0.5f;
	float lacunarity = 2.0f;
	float frequency = 0.05f;

	void ApplyConfig(FastNoiseLite& noise)
	{
		noise.SetNoiseType(static_cast<FastNoiseLite::NoiseType>(type));
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetSeed(seed);
		noise.SetFractalGain(gain);
		noise.SetFractalLacunarity(lacunarity);
		noise.SetFractalOctaves(octaves);
		noise.SetFrequency(frequency);
	}
};

struct Point
{
	float x, y, z, w;
};

struct Triangle
{
	Point points[3];
};

struct TriEntry
{
	int edge[16];
};

using TriangleBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, Triangle, GL_DYNAMIC_COPY>;
using FloatBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, float, GL_DYNAMIC_DRAW>;
using AtomicCounter = Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned, GL_DYNAMIC_DRAW>;
using TriLUTBuffer = Buffer<GL_SHADER_STORAGE_BUFFER, int[16], GL_STATIC_DRAW>;

#define MC_DIM 10

class MarchingCubesGame : public FirstPersonGame
{
	std::shared_ptr<Primitive> m_Terrain;
	std::shared_ptr<Material> m_TerrainMaterial;
	FastNoiseLite m_Noise;
	NoiseConfig m_NoiseConfig;
	MC::McConfig m_TerrainConfig;
	bool m_RefreshTerrain = true;

	std::shared_ptr<Primitive> m_Skull;
	std::vector<char> m_SkullData;
	MC::McConfig m_SkullConfig{16, 0.1f, 80.0f};
	bool m_RefreshSkull = true;

	std::shared_ptr<TriangleBuffer> triangleBuffer;
	std::shared_ptr<AtomicCounter> atomicCounter;
	std::shared_ptr<FloatBuffer> densityBuffer;
	std::shared_ptr<TriLUTBuffer> triLUTBuffer;
	std::shared_ptr<Shader> computeShader;
	std::shared_ptr<Shader> mcShader;

	unsigned int vCount = 0;

public:
	MarchingCubesGame(unsigned int width, unsigned int height, const std::string& title)
		: FirstPersonGame(width, height, title),
		  m_SkullData(FileSystem::ReadBytesFromFile("skull.raw", 256 * 256 * 256))
	{
		m_TerrainMaterial = std::make_shared<Material>(std::make_shared<Shader>(
			"shaders/MarchingCubes.vert", "shaders/MarchingCubes.frag"));

		int workGroupCount[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCount[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCount[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCount[2]);
		printf("max global (total) work group counts x:%i y:%i z:%i\n", workGroupCount[0],
			workGroupCount[1], workGroupCount[2]);

		int workGroupSize[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);
		printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
			workGroupSize[0], workGroupSize[1], workGroupSize[2]);

		int workGroupInvocations;
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workGroupInvocations);
		printf("max local work group invocations %i\n", workGroupInvocations);

		triangleBuffer =
			std::make_shared<TriangleBuffer>(MC_DIM * MC_DIM * MC_DIM, nullptr);

		atomicCounter = std::make_shared<AtomicCounter>(1, &vCount);

		computeShader = std::make_shared<Shader>("shaders/MarchingCubes.comp");
		mcShader = std::make_shared<Shader>("shaders/MarchingCubes2.vert",
			"shaders/MarchingCubes2.frag");

		m_NoiseConfig.ApplyConfig(m_Noise);
		float* densities = new float[MC_DIM * MC_DIM * MC_DIM];
		for (int x = 0; x < MC_DIM; ++x)
		{
			for (int y = 0; y < MC_DIM; ++y)
			{
				for (int z = 0; z < MC_DIM; ++z)
				{
					float xf = x;
					float yf = y;
					float zf = z;
					densities[x + (y * MC_DIM) + (z * MC_DIM * MC_DIM)] =
						m_Noise.GetNoise(static_cast<float>(xf), static_cast<float>(yf),
							static_cast<float>(zf));
				}
			}
		}
		
		std::cout << "densities done\n";

		densityBuffer =
			std::make_shared<FloatBuffer>(MC_DIM * MC_DIM * MC_DIM, densities);

		triLUTBuffer = std::make_shared<TriLUTBuffer>(256, triangleLUT);

		glBindBufferBase(triangleBuffer->GetBufferType(), 4, triangleBuffer->GetHandle());
		glBindBufferBase(densityBuffer->GetBufferType(), 5, densityBuffer->GetHandle());
		glBindBufferBase(atomicCounter->GetBufferType(), 6, atomicCounter->GetHandle());
		glBindBufferBase(triLUTBuffer->GetBufferType(), 7, triLUTBuffer->GetHandle());
		computeShader->Bind();
		computeShader->SetFloat("isoValue", 0.0f);
		glDispatchCompute(MC_DIM / 10, MC_DIM / 10, MC_DIM / 10);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		atomicCounter->Bind();
		vCount = *(unsigned int*) glMapBufferRange(atomicCounter->GetBufferType(), 0,
			sizeof(GLuint), GL_MAP_READ_BIT);
		glUnmapBuffer(atomicCounter->GetBufferType());

		std::cout << vCount << '\n';
	}

	void Update(float delta) override
	{
		FirstPersonGame::Update(delta);

		RefreshMeshes();
		DrawMeshes();
		ShowStats();
		ShowTerrainWindow();
		ShowSkullWindow();
	}

private:
	void RefreshMeshes()
	{
		if (m_RefreshTerrain)
		{
			m_RefreshTerrain = false;

			m_NoiseConfig.ApplyConfig(m_Noise);

			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			MC::MarchingCubes(m_TerrainConfig, vertices, indices,
				[&](glm::vec3 pos) -> float
				{ return -pos.y + m_Noise.GetNoise(pos.x, pos.y, pos.z); });

			VertexBufferLayout layout;
			layout.Push<float>(3);

			m_Terrain =
				std::make_shared<Primitive>(vertices, indices, layout, m_TerrainMaterial);
			m_Terrain->GetMaterial()->SetUniform("projection", m_Projection);
		}

		if (m_RefreshSkull)
		{
			m_RefreshSkull = false;

			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			MC::MarchingCubes(m_SkullConfig, vertices, indices,
				[&](glm::vec3 pos) -> float
				{
					return static_cast<float>(m_SkullData[static_cast<int>(
						pos.x + pos.y * 256 + pos.z * 256 * 256)]);
				});

			VertexBufferLayout layout;
			layout.Push<float>(3);
			m_Skull =
				std::make_shared<Primitive>(vertices, indices, layout, m_TerrainMaterial);
		}
	}

	void DrawMeshes()
	{
		m_Terrain->GetMaterial()->SetUniform("view", m_View);
		m_Terrain->GetMaterial()->SetUniform("modulate",
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		m_Terrain->Draw(
			glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 10.0f, -10.0f)));

		m_Terrain->GetMaterial()->SetUniform("modulate",
			glm::vec4(0.0f, 1.0f, 0.5f, 1.0f));
		glm::mat4 skullModel =
			glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)),
				glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_Skull->Draw(skullModel);

		mcShader->Bind();
		mcShader->SetMat4("projection", m_Projection);
		mcShader->SetMat4("view", m_View);
		// mcShader->SetMat4("model", m_Projection);
		triangleBuffer->Bind();

		glDrawArrays(GL_TRIANGLES, 0, vCount * 3);
	}

	void ShowStats()
	{
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowBgAlpha(0.15f);
		ImGui::Begin("Performance Stats", 0, window_flags);
		ImGui::Text("Frame Time: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x,
				ImGui::GetIO().MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		ImGui::End();
	}

	void ShowTerrainWindow()
	{
		ImGui::SetNextWindowCollapsed(!m_Window->IsCursorEnabled());
		ImGui::Begin("Terrain Settings", 0,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		if (m_Window->IsCursorEnabled())
		{
			ImGui::Text("Marching Cubes Settings");
			m_RefreshTerrain |=
				ImGui::SliderInt("Subdivisions", &m_TerrainConfig.subdivisions, 1, 100);
			m_RefreshTerrain |=
				ImGui::SliderFloat("Scale", &m_TerrainConfig.scale, 0.0f, 10.0f);
			m_RefreshTerrain |=
				ImGui::SliderFloat("Surface", &m_TerrainConfig.surface, -20.0f, 20.0f);

			ImGui::Text("Noise Settings");
			static const char* items[] = {"OpenSimplex2", "OpenSimplex2S", "Cellular",
				"Perlin", "ValueCubic", "Value"};
			m_RefreshTerrain |= ImGui::Combo("Type", &m_NoiseConfig.type, items, 6);
			m_RefreshTerrain |= ImGui::DragInt("Seed", &m_NoiseConfig.seed);
			m_RefreshTerrain |=
				ImGui::SliderInt("Octaves", &m_NoiseConfig.octaves, 0, 20);
			m_RefreshTerrain |=
				ImGui::SliderFloat("Gain", &m_NoiseConfig.gain, 0.0f, 2.0f);
			m_RefreshTerrain |=
				ImGui::SliderFloat("Lacunarity", &m_NoiseConfig.lacunarity, 0.0f, 5.0f);
			m_RefreshTerrain |=
				ImGui::SliderFloat("Frequency", &m_NoiseConfig.frequency, 0.0f, 0.2f);
		}
		ImGui::End();
	}

	void ShowSkullWindow()
	{
		ImGui::SetNextWindowCollapsed(!m_Window->IsCursorEnabled());
		ImGui::Begin("Skull Settings", 0,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		if (m_Window->IsCursorEnabled())
		{
			m_RefreshSkull |=
				ImGui::InputFloat("Surface", &m_SkullConfig.surface, 10.0f, 10.0f);
		}
		ImGui::End();
	}
};

int main()
{
	MarchingCubesGame(1920, 1080, "MyGame").Start();
	return 0;
}
