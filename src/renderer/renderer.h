#pragma once

#include "common/common.h"
#include <GL/glew.h>
#include <string>

class Renderer
{
private:
	static GLuint vao;
	static GLuint tex;
	static dvec2 size;
	static double rot;
	static bool screenCoord;
public:
	static void init();
	static void clear();

	static void setSize(const dvec2 & v);
	static void setRotation(double v);
	static void setTexture(GLuint v);
	static void setTexture(const std::string & name);
	static void setScreenCoord(bool v);

	static void render(const dvec2 & pos);
};