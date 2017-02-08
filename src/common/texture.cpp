#include "texture.h"

Texture::Texture()
{
	glGenTextures(1, &mId);
}

Texture::~Texture()
{
}

void Texture::generate(const unsigned char * image, int w, int h, bool alpha)
{
	glBindTexture(GL_TEXTURE_2D, mId);

	// NOTE: setting texture filter parameters is required, or render will fail 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, w, h, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getId() const
{
	return mId;
}

void Texture::clear()
{
	glDeleteTextures(1, &mId);
}