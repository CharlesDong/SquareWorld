#pragma once

#include <GL/glew.h>

class Texture
{
private:
	GLuint mId;
public:
	Texture();
	~Texture();

	void generate(const unsigned char * image, int w, int h, bool alpha);
	GLuint getId() const;

	void clear();
};