#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
	unsigned int m_Handle;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string &vsPath, const std::string &fsPath);
	explicit Shader(const std::string &csPath);
	~Shader();
	
	void Bind() const;
	
	void SetInt(const std::string &name, int value);
	void SetFloat(const std::string &name, float value);
	void SetVec2(const std::string &name, const glm::vec2 &value);
	void SetVec3(const std::string &name, const glm::vec3 &value);
	void SetVec4(const std::string &name, const glm::vec4 &value);
	void SetMat2(const std::string &name, const glm::mat2 &mat);
	void SetMat3(const std::string &name, const glm::mat3 &mat);
	void SetMat4(const std::string &name, const glm::mat4 &mat);

private:
	[[nodiscard]] int GetUniformLocation(const std::string &name);
	static unsigned int Compile(const std::string &source, GLenum type);
};
