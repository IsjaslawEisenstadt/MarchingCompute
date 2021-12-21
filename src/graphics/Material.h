#pragma once

#include <memory>
#include <variant>

#include <glm/glm.hpp>

#include "Shader.h"

class Material
{
	enum class UniformType
	{
		INT,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4
	};

	struct UniformValue
	{
		std::variant<int,
					 float,
					 glm::vec2,
					 glm::vec3,
					 glm::vec4,
					 glm::mat2,
					 glm::mat3,
					 glm::mat4>
			value;
		UniformType type;
	};

	Shader m_Shader;
	std::unordered_map<std::string, UniformValue> m_Uniforms;

	bool m_TwoSided = true;

public:
	Material() {}
	explicit Material(Shader &&shader) : m_Shader(std::move(shader)) {}

	void Bind();

	inline const Shader &GetShader() const { return m_Shader; }
	inline void SetShader(Shader &&shader) { m_Shader = std::move(shader); }

	inline bool IsTwoSided() const { return m_TwoSided; }
	inline void SetTwoSided(bool twoSided) { m_TwoSided = twoSided; }

	template<typename T>
	[[maybe_unused]] const T &GetUniform(const std::string &name) const
	{
		return m_Uniforms.at(name);
	}

	template<typename T>
	void SetUniform(const std::string &name, T value)
	{
		static_assert("Unsupported SetUniform Type");
	}

	template<>
	[[maybe_unused]] void SetUniform<int>(const std::string &name, int value)
	{
		m_Uniforms[name] = {value, UniformType::INT};
	}

	template<>
	[[maybe_unused]] void SetUniform<float>(const std::string &name, float value)
	{
		m_Uniforms[name] = {value, UniformType::FLOAT};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::vec2>(const std::string &name, glm::vec2 value)
	{
		m_Uniforms[name] = {value, UniformType::VEC2};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::vec3>(const std::string &name, glm::vec3 value)
	{
		m_Uniforms[name] = {value, UniformType::VEC3};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::vec4>(const std::string &name, glm::vec4 value)
	{
		m_Uniforms[name] = {value, UniformType::VEC4};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::mat2>(const std::string &name, glm::mat2 value)
	{
		m_Uniforms[name] = {value, UniformType::MAT2};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::mat3>(const std::string &name, glm::mat3 value)
	{
		m_Uniforms[name] = {value, UniformType::MAT3};
	}

	template<>
	[[maybe_unused]] void SetUniform<glm::mat4>(const std::string &name, glm::mat4 value)
	{
		m_Uniforms[name] = {value, UniformType::MAT4};
	}
};
