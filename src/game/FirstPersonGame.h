#pragma once

#include "core/BaseGame.h"
#include "core/Constants.h"
#include "graphics/Camera.h"

class FirstPersonGame : public BaseGame
{
	std::shared_ptr<Material> m_Material;
	Primitive m_Origin;

	glm::vec3 m_CameraMoveDirection{0.0f};
	glm::vec2 m_MousePosition{0.0f};
	glm::vec2 m_MouseDelta{0.0f};

	struct ViewProjection
	{
		glm::mat4 view;
		glm::mat4 projection;
	};

	using UniformBuffer = Buffer<GL_UNIFORM_BUFFER, ViewProjection, GL_DYNAMIC_DRAW>;

	UniformBuffer viewProjectionBuffer;

protected:
	Camera m_Camera;
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	bool mouseDirtyFlag = true;

public:
	FirstPersonGame(unsigned int width, unsigned int height, const std::string &title);

	void Update(float delta) override;
	void OnKeyInput(int key, int scancode, int action, int mods) override;
	void OnMouseMotionInput(double xpos, double ypos) override;
	void OnMouseButtonInput(int button, int action, int mods) override;
};
