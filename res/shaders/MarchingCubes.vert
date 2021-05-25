#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

out vec4 ve;

void main()
{
	ve = model * vec4(aPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
