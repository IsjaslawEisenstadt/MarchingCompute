#pragma once

#include <string>

#include <glad/glad.h>

struct TextureParams
{
	int wrap_s = GL_REPEAT;
	int wrap_t = GL_REPEAT;
	int min_filter = GL_LINEAR;
	int mag_filter = GL_LINEAR;
	bool mipmap = true;
};

class Texture
{
	unsigned int m_Handle;
	unsigned int m_Type;
	int m_Width;
	int m_Height;
	int m_NumChannels;

public:
	explicit Texture(const std::string &path, TextureParams params = {});
	~Texture();

	void Bind(unsigned int slot = 0) const;
};
