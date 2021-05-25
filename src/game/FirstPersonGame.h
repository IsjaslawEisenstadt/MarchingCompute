#pragma once

#include "core/BaseGame.h"
#include "core/Constants.h"
#include "graphics/Camera.h"

class FirstPersonGame : public BaseGame
{
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<Primitive> m_Floor;
	std::shared_ptr<Primitive> m_Cube;
	std::shared_ptr<Primitive> m_Billboard;
	std::shared_ptr<Primitive> m_Origin;

	std::shared_ptr<Camera> m_Camera;
	glm::vec3 m_CameraMoveDirection{0.0f};
	glm::vec2 m_MousePosition{0.0f};
	glm::vec2 m_MouseDelta{0.0f};

protected:
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	bool dirty = true;

public:
	FirstPersonGame(unsigned int width, unsigned int height, const std::string& title);

	void Update(float delta) override;
	void OnKeyInput(int key, int scancode, int action, int mods) override;
	void OnMouseMotionInput(double xpos, double ypos) override;
	void OnMouseButtonInput(int button, int action, int mods) override;
};
