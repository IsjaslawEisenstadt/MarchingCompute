#pragma once

#include <glad/glad.h>

#include "graphics/Renderer.h"
#include "InputListener.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>

enum class InputType
{
	KEYBOARD,
	MOUSE
};

class Window
{
	GLFWwindow *m_Window;
	bool m_VSync = true;
	bool m_FrameCapEnabled = false;
	unsigned int m_FrameCap = 144;
	std::vector<std::reference_wrapper<InputListener>> listeners;

public:
	Window(unsigned int width, unsigned int height, const std::string &title);
	~Window();

	[[nodiscard]] static inline float GetTime()
	{
		return static_cast<float>(glfwGetTime());
	}
	[[nodiscard]] inline bool ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
	inline void PresentFrame() const { glfwSwapBuffers(m_Window); }
	inline void PollInput() const { glfwPollEvents(); }

	void AddInputListener(InputListener &inputListener);
	inline void RemoveInputListener(InputListener &inputListener);

	void SetVSyncEnabled(bool enabled);
	[[nodiscard]] inline bool IsVSyncEnabled() const { return m_VSync; }
	inline void SetFrameCapEnabled(bool enabled) { m_FrameCapEnabled = enabled; }
	[[nodiscard]] inline bool IsFrameCapEnabled() const { return m_FrameCapEnabled; }
	inline void SetFrameCap(unsigned int frameCap) { m_FrameCap = frameCap; }
	[[nodiscard]] inline unsigned int GetFrameCap() const { return m_FrameCap; }

	[[nodiscard]] inline bool IsCursorEnabled() const
	{
		return glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}
	void SetCursorEnabled(bool enabled);
	
	inline glm::vec2 GetMousePosition() const
	{
		double mx, my;
		glfwGetCursorPos(m_Window, &mx, &my);
		return glm::vec2(mx, my);
	}

	inline void Quit() { glfwSetWindowShouldClose(m_Window, GLFW_TRUE); }
	inline GLFWwindow *GetPointer() const { return m_Window; }
};