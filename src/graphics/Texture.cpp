#include "Texture.h"

#include <iostream>

#include <stb_image.h>

Texture::Texture(const std::string &path, TextureParams params) : m_Type(GL_TEXTURE_2D)
{
	glGenTextures(1, &m_Handle);
	glBindTexture(m_Type, m_Handle);
	glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, params.wrap_s);
	glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, params.wrap_t);
	glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, params.min_filter);
	glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, params.mag_filter);

	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NumChannels, 0);
	if (data)
	{
		glTexImage2D(m_Type,
					 0,
					 GL_RGB,
					 m_Width,
					 m_Height,
					 0,
					 GL_RGB,
					 GL_UNSIGNED_BYTE,
					 data);
		if (params.mipmap)
		{
			glGenerateMipmap(m_Type);
		}
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &m_Handle); }

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_Type, m_Handle);
}
