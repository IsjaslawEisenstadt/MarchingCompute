#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

layout (std140, binding = 0) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model = mat4(1.0f);

void main()
{
	vColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
