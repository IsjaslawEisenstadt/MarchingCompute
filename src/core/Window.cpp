#include "Window.h"
#include <iostream>

Window::Window(unsigned int width, unsigned int height, const std::string &title)
{
	glfwSetErrorCallback([](int error, const char *description)
		{ std::cerr << "GLFW error " << error << ": " << description << '\n'; });

	if (!glfwInit())
	{
		std::cerr << "GLFW init failed\n";
		abort();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_Window)
	{
		std::cerr << "Window creation failed\n";
		glfwTerminate();
		abort();
	}

	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize OpenGL Loader\n";
		abort();
	}

	int profileBit;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileBit);

	std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " "
			  << (profileBit == 1 ? "Core" : "Compatibility") << '\n';

	SetVSyncEnabled(m_VSync);
	SetCursorEnabled(true);

	glfwSetWindowUserPointer(m_Window, this);

	glfwSetKeyCallback(m_Window,
		[](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			for (auto &listener :
				((Window *) glfwGetWindowUserPointer(window))->listeners)
			{
				listener.get().OnKeyInput(key, scancode, action, mods);
			}
		});
	glfwSetMouseButtonCallback(m_Window,
		[](GLFWwindow *window, int button, int action, int mods)
		{
			for (auto &listener :
				((Window *) glfwGetWindowUserPointer(window))->listeners)
			{
				listener.get().OnMouseButtonInput(button, action, mods);
			}
		});
	glfwSetCursorPosCallback(m_Window,
		[](GLFWwindow *window, double xpos, double ypos)
		{
			for (auto &listener :
				((Window *) glfwGetWindowUserPointer(window))->listeners)
			{
				listener.get().OnMouseMotionInput(xpos, ypos);
			}
		});
	glfwSetScrollCallback(m_Window,
		[](GLFWwindow *window, double xoffset, double yoffset)
		{
			for (auto &listener :
				((Window *) glfwGetWindowUserPointer(window))->listeners)
			{
				listener.get().OnScrollInput(xoffset, yoffset);
			}
		});
}

Window::~Window() { glfwTerminate(); }

void Window::AddInputListener(InputListener &inputListener)
{
	listeners.push_back(inputListener);
}

void Window::RemoveInputListener(InputListener &inputListener)
{
	// listeners.erase(std::remove(listeners.begin(), listeners.end(), inputListener),
	// listeners.end());
}

void Window::SetVSyncEnabled(bool enabled)
{
	m_VSync = enabled;
	glfwSwapInterval(m_VSync);
}

void Window::SetCursorEnabled(bool enabled)
{
	glfwSetInputMode(m_Window, GLFW_CURSOR,
		enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
