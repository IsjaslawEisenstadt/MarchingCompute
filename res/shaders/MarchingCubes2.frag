#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform struct Material {
	vec3 objectColor;
	float shininess;
} material;

uniform struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} dirLight;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 lightIntensity = dirLight.ambient + dirLight.diffuse * diff + dirLight.specular * spec;
	FragColor = vec4(lightIntensity * material.objectColor, 1.0);
}
