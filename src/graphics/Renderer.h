#pragma once

#include "Buffer.h"
#include "Material.h"
#include "Primitive.h"
#include "Shader.h"

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

class Renderer
{
public:
	// this class is purely static for now
	Renderer() = delete;

	static void Init();
	static void Clear(Color color = {0.2f, 0.3f, 0.3f, 1.0f});
};
