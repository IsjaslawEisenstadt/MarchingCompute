#version 460 core

struct Triangle
{
	vec4 points[3];
};

layout(std430, binding = 4) buffer TriangleBuffer
{
	Triangle triangles[];
};

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

void main()
{
	gl_Position = projection * view * model * triangles[gl_VertexID / 3].points[gl_VertexID % 3];
}
