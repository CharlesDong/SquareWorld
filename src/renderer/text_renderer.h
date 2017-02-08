#pragma once

#include "common/common.h"
#include <GL/glew.h>

class TextRenderer
{
private:
	struct Character
	{
		GLuint id;
		ivec2 size;
		ivec2 bearing;
		int advance;
	};
	static Character chars[128];
	static GLuint vao;

	static void drawChar(int x, int y, char ch);
	static void drawString(int x, int y, const char * text);
public:
	static void init();
	static void text(int x, int y, const char * fmt, ...);

	static int maxHeight();
	static int length(const char * text);
};