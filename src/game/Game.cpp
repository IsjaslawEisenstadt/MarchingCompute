#include <fast_noise_lite/FastNoiseLite.h>
#include <imgui/imgui.h>

#include "core/FileSystem.h"
#include "game/FirstPersonGame.h"
#include "terrain/VoxelTerrain.h"

struct NoiseConfig
{
	int type = FastNoiseLite::NoiseType_OpenSimplex2;
	int seed = 1337;
	int octaves = 3;
	float gain = 0.5f;
	float lacunarity = 2.0f;
	float frequency = 0.05f;

	void ApplyConfig(FastNoiseLite &noise)
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

class MarchingCubesGame : public FirstPersonGame
{
	std::shared_ptr<VoxelMesh> m_Terrain;
	FastNoiseLite m_Noise;
	NoiseConfig m_NoiseConfig;
	MarchingCubesConfig m_TerrainConfig;
	bool m_RefreshTerrain = true;

public:
	MarchingCubesGame(unsigned int width, unsigned int height, const std::string &title)
		: FirstPersonGame(width, height, title)
	{
		m_TerrainConfig.densityFunc = [&](glm::vec3 pos)
		{ return m_Noise.GetNoise(pos.x, pos.y, pos.z); };

		m_Terrain = std::make_shared<VoxelMesh>(m_TerrainConfig);
	}

	void Update(float delta) override
	{
		FirstPersonGame::Update(delta);

		RefreshMeshes();
		DrawMeshes();
		ShowStats();
		ShowTerrainWindow();
	}

private:
	void RefreshMeshes()
	{
		if (m_RefreshTerrain)
		{
			m_RefreshTerrain = false;

			m_NoiseConfig.ApplyConfig(m_Noise);

			m_Terrain->Regenerate(m_TerrainConfig);
		}
	}

	void DrawMeshes()
	{
		m_Terrain->Draw(
			glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 10.0f, -10.0f)));
	}

	void ShowStats()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
										| ImGuiWindowFlags_AlwaysAutoResize
										| ImGuiWindowFlags_NoFocusOnAppearing
										| ImGuiWindowFlags_NoNav
										| ImGuiWindowFlags_NoTitleBar
										| ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowBgAlpha(0.15f);
		ImGui::Begin("Performance Stats", 0, window_flags);
		ImGui::Text("Frame Time: %.3f ms (%.1f FPS)",
					1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)",
						ImGui::GetIO().MousePos.x,
						ImGui::GetIO().MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		ImGui::End();
	}

	void ShowTerrainWindow()
	{
		ImGui::SetNextWindowCollapsed(!m_Window.IsCursorEnabled());
		ImGui::Begin("Terrain Settings",
					 0,
					 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		if (m_Window.IsCursorEnabled())
		{
			ImGui::Text("Marching Cubes Settings");

			m_RefreshTerrain |=
				ImGui::SliderFloat("Scale", &m_TerrainConfig.scale, 0.01f, 10.0f);

			m_RefreshTerrain |= ImGui::SliderFloat("IsoValue",
												   &m_TerrainConfig.isoValue,
												   0.0f,
												   1.0f);

			ImGui::Text("Noise Settings");
			static const char *items[] = {"OpenSimplex2",
										  "OpenSimplex2S",
										  "Cellular",
										  "Perlin",
										  "ValueCubic",
										  "Value"};
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
};

int main()
{
	MarchingCubesGame(1920, 1080, "MyGame").Start();
	return 0;
}
