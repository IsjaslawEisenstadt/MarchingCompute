#include "FirstPersonGame.h"

#include <glm/gtc/matrix_transform.hpp>

FirstPersonGame::FirstPersonGame(unsigned int width,
								 unsigned int height,
								 const std::string &title)
	: BaseGame(width, height, title),
	  m_MousePosition(m_Window.GetMousePosition()),
	  m_Camera(glm::vec3(150.0f, 150.0f, 150.0f), glm::vec3(0.0f, 1.0f, 0.0f), 225.0f, -30.0f)
{
	Shader shader("shaders/Default.vert", "shaders/Default.frag");
	m_Material = std::make_shared<Material>(std::move(shader));

	VertexBufferLayout layout;
	layout.Push<float>(3).Push<float>(3);

	m_Origin.SetVertices(originVertices, layout);
	m_Origin.SetMaterial(m_Material);
	m_Origin.SetMode(GL_LINES);

	glm::mat4 view(1.0f);
	m_Projection = glm::perspective(glm::radians(75.0f),
									static_cast<float>(width)
										/ static_cast<float>(height),
									0.1f,
									5000.0f);

	viewProjectionBuffer.BufferData(1, nullptr);
	viewProjectionBuffer.Bind();
	glBufferSubData(GL_UNIFORM_BUFFER,
					sizeof(glm::mat4),
					sizeof(glm::mat4),
					&m_Projection);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, viewProjectionBuffer.GetHandle());
}

void FirstPersonGame::Update(float delta)
{
	glm::vec3 move(0.0f);
	if (glm::length(m_CameraMoveDirection) >= 0.001f)
	{
		move = glm::normalize(m_CameraMoveDirection);
	}
	m_Camera.Move(move * delta * 35.0f);
	m_Camera.Rotate(glm::vec3(m_MouseDelta.y, m_MouseDelta.x, 0.0f) * 0.25f);
	m_MouseDelta = glm::vec2(0.0f);

	m_View = m_Camera.GetView();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_View);

	Renderer::Clear();
	m_Origin.Draw();

	glm::mat4 billboardModel = glm::translate(glm::mat4(1.0f),
											  glm::vec3(5.0f, 0.0f, -5.0f));

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			billboardModel[i][j] = m_View[j][i];
		}
	}
}

void FirstPersonGame::OnKeyInput(int key, int scancode, int action, int mods)
{
	float actionDir = action == GLFW_PRESS ? 1.0f
										   : (action == GLFW_RELEASE ? -1.0f : 0.0f);
	if (actionDir)
	{
		switch (key)
		{
			case GLFW_KEY_W:
				m_CameraMoveDirection += glm::vec3(0.0f, 0.0f, actionDir);
				break;
			case GLFW_KEY_S:
				m_CameraMoveDirection -= glm::vec3(0.0f, 0.0f, actionDir);
				break;
			case GLFW_KEY_A:
				m_CameraMoveDirection -= glm::vec3(actionDir, 0.0f, 0.0f);
				break;
			case GLFW_KEY_D:
				m_CameraMoveDirection += glm::vec3(actionDir, 0.0f, 0.0f);
				break;
			case GLFW_KEY_Q:
				m_CameraMoveDirection += glm::vec3(0.0f, actionDir, 0.0f);
				break;
			case GLFW_KEY_E:
				m_CameraMoveDirection -= glm::vec3(0.0f, actionDir, 0.0f);
				break;
			case GLFW_KEY_ESCAPE: m_Window.Quit();
		}
	}
}

void FirstPersonGame::OnMouseMotionInput(double xpos, double ypos)
{
	if (!m_Window.IsCursorEnabled())
	{
		if (mouseDirtyFlag)
		{
			m_MousePosition = glm::vec2(xpos, ypos);
			m_MouseDelta = glm::vec2(0.0f);
			mouseDirtyFlag = false;
		}
		else
		{
			glm::vec2 newMousePosition(xpos, ypos);
			m_MouseDelta = newMousePosition - m_MousePosition;
			m_MousePosition = newMousePosition;
		}
	}
}

void FirstPersonGame::OnMouseButtonInput(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			m_Window.SetCursorEnabled(false);
		}
		else if (action == GLFW_RELEASE)
		{
			m_Window.SetCursorEnabled(true);
			mouseDirtyFlag = true;
		}
	}
}
