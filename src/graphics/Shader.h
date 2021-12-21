#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
	unsigned int m_Handle = 0;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader();
	Shader(const std::string &vsPath, const std::string &fsPath);
	explicit Shader(const std::string &csPath);

	~Shader();

	Shader(const Shader &other) = delete;
	Shader &operator=(const Shader &other) = delete;

	Shader(Shader &&other)
		: m_Handle(other.m_Handle),
		  m_UniformLocationCache(std::move(other.m_UniformLocationCache))
	{
		other.m_Handle = 0;
	}

	Shader &operator=(Shader &&other)
	{
		DestroyShader();

		m_Handle = other.m_Handle;
		other.m_Handle = 0;
		m_UniformLocationCache = std::move(other.m_UniformLocationCache);
		return *this;
	}

	void LoadShader(const std::string &vsPath, const std::string &fsPath);
	void LoadShader(const std::string &csPath);

	void Bind() const;

	inline void DestroyShader()
	{
		glDeleteProgram(m_Handle);
		m_Handle = 0;
	}

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
