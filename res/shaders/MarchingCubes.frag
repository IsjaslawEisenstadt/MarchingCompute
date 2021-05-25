#version 460 core

out vec4 FragColor;

in vec4 ve;
uniform mat4 model = mat4(1.0f);
uniform vec4 modulate;

void main() {
	FragColor = vec4(sin(ve.y), 1.0f, 1.0f, 1.0f) * modulate;
}
