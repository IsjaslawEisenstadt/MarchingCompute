#include "Material.h"

void Material::Bind()
{
	m_Shader->Bind();

	for (auto &m_Uniform : m_Uniforms)
	{
		const UniformValue &uniform = m_Uniform.second;
		switch (uniform.type)
		{
			case UniformType::INT:
				m_Shader->SetInt(m_Uniform.first, std::get<int>(uniform.value));
				break;
			case UniformType::FLOAT:
				m_Shader->SetFloat(m_Uniform.first, std::get<float>(uniform.value));
				break;
			case UniformType::VEC2:
				m_Shader->SetVec2(m_Uniform.first, std::get<glm::vec2>(uniform.value));
				break;
			case UniformType::VEC3:
				m_Shader->SetVec3(m_Uniform.first, std::get<glm::vec3>(uniform.value));
				break;
			case UniformType::VEC4:
				m_Shader->SetVec4(m_Uniform.first, std::get<glm::vec4>(uniform.value));
				break;
			case UniformType::MAT2:
				m_Shader->SetMat2(m_Uniform.first, std::get<glm::mat2>(uniform.value));
				break;
			case UniformType::MAT3:
				m_Shader->SetMat3(m_Uniform.first, std::get<glm::mat3>(uniform.value));
				break;
			case UniformType::MAT4:
				m_Shader->SetMat4(m_Uniform.first, std::get<glm::mat4>(uniform.value));
				break;
		}
	}
}
