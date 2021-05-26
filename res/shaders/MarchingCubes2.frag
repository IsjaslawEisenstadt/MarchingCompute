#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
} dirLight;

uniform vec3 objectColor;

void main()
{
	float diffuse = max(dot(normalize(Normal), -dirLight.direction), 0.0);
	FragColor = vec4((dirLight.ambient + dirLight.diffuse * diffuse) * objectColor, 1.0);
}
