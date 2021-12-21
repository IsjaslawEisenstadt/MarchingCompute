#include "Renderer.h"

#include <iostream>

static void GLAPIENTRY MessageCallback(GLenum source,
									   GLenum type,
									   GLuint id,
									   GLenum severity,
									   GLsizei length,
									   const char *message,
									   const void *userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && severity != GL_DEBUG_SEVERITY_LOW)
		std::cout << "OpenGL message: " << message << '\n';
}

void Renderer::Init()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CW);

	// glViewport(0, 0, width, height);
}

void Renderer::Clear(Color color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
