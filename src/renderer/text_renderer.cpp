#include "text_renderer.h"
#include "common/resource_manager.h"
#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <cstdarg>
#include <sstream>

#include <ft2build.h>
#include FT_FREETYPE_H		// <freetype/freetype.h>

TextRenderer::Character TextRenderer::chars[128];
GLuint TextRenderer::vao;

// Init Freetype, load fonts, setting up vertex data, etc.
void TextRenderer::init()
{
	// init FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		fatals << "Failed to initialize FreeType!\n";
	}

	// check if the font file exist
	std::ifstream fin("res/fonts/consola.ttf");
	if (!fin.is_open())
	{
		fatals << "Cannot open font file fonts/consola.ttf\n";
	}
	else
		fin.close();

	// create a Freetype face
	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/consola.ttf", 0, &face))
	{
		fatals << "Failed to load FreeType font!\n";
	}

	FT_Set_Pixel_Sizes(face, 0, 18);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load glyphs
	int c;
	GLuint texture;
	for (c = 0; c <= 127; c++)
	{
		// load character
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			fatals << "Failed to load glyph, char code: " << (int)c << "\n";
		}

		// OpenGL texture settings
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// glyph data
		Character & ch = chars[c];
		ch.id = texture;
		ch.size = ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		ch.bearing = ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		ch.advance = face->glyph->advance.x >> 6;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// free resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	GLfloat vertices[] =
	{
		// pos		// tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	ResourceManager::getShader("text").use();
	glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	ResourceManager::getShader("text").set4x4f("projection", proj);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	ResourceManager::getShader("text").set3f("textColor", color);
}

// A macro used in draw() to draw VAs' values.
#define DrawArg(type)			\
	ostr << *((type *)ap);		\
	str = ostr.str();			\
	cstr = str.c_str();			\
	drawString(x, y, cstr);		\
	va_arg(ap, type)

// Draw a line of text. '%' in "format" will be regarded as the format character.
// TODO: improve format string analyzing
void TextRenderer::text(int x, int y, const char * format, ...)
{
	ResourceManager::getShader("text").use();

	int i;
	va_list ap;
	std::ostringstream ostr;
	std::string str;
	const char * cstr;
	ostr.setf(std::ios_base::boolalpha);

	va_start(ap, format);
	for (i = 0; format[i]; i++)
	{
		if (format[i] != '%')
			drawChar(x, y, format[i]);
		else
		{
			i++;
			if (format[i] == '%')
				drawChar(x, y, format[i]);
			else
			{
				switch (format[i])
				{
				case 'd':
				case 'D':
					DrawArg(int);
					break;
				case 'l':
				case 'L':
					i++;
					switch (format[i])
					{
					case 'f':
					case 'F':
						DrawArg(double);
						break;
					}
					break;
				case 'b':
				case 'B':
					DrawArg(bool);
					break;
				case 's':
				case 'S':
					DrawArg(char *);
					break;
				}
			}
		}
		x += chars[format[i]].advance;
	}
	va_end(ap);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Draw a single character, called by text().
void TextRenderer::drawChar(int x, int y, char ch)
{
	using glm::mat4;
	using glm::translate;
	using glm::scale;

	int xpos, ypos;
	xpos = x + chars[ch].bearing.x;
	ypos = y + (chars[ch].bearing.y - chars[ch].size.y);

	mat4 model;
	model = translate(model, vec3(xpos, ypos, 1.0f));
	model = scale(model, vec3(chars[ch].size, 1.0f));
	ResourceManager::getShader("text").set4x4f("model", model);

	glBindTexture(GL_TEXTURE_2D, chars[ch].id);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// Draw a string with no format string.
void TextRenderer::drawString(int x, int y, const char * text)
{
	int i;
	for (i = 0; text[i]; i++)
	{
		drawChar(x, y, text[i]);
		x += chars[text[i]].advance;
	}
}

// Get the maximum height of a line of text.
int TextRenderer::maxHeight()
{
	return chars['T'].bearing.y;
}

int TextRenderer::length(const char * text)
{
	int res = 0;
	int i;
	for (i = 0; i < (int)strlen(text); i++)
		res += chars[text[i]].advance;
	res -= chars[text[i - 1]].advance;
	return res;
}