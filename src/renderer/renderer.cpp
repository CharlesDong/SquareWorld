#include "renderer.h"

#include "common/resource_manager.h"
#include "common/common.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

GLuint Renderer::vao;
GLuint Renderer::tex;		// OpenGL texture ID
dvec2 Renderer::size;
double Renderer::rot;
bool Renderer::screenCoord;

using glm::vec2;
using glm::vec3;
using glm::mat4;

void Renderer::init()
{
	vao = 0;
	tex = 0;
	size = dvec2(0.0, 0.0);
	rot = 0.0;
	screenCoord = false;

	GLuint vbo;
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
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mat4 projection = glm::ortho(0.0f, 800.0f / (float)BLOCKSIZE, 0.0f, 600.0f / (float)BLOCKSIZE);
	ResourceManager::getShader("shader").use();
	ResourceManager::getShader("shader").set4x4f("projection", projection);
}

void Renderer::clear()
{
	glDeleteVertexArrays(1, &vao);
}

void Renderer::render(const dvec2 & pos)
{
	using glm::translate;
	using glm::rotate;
	using glm::scale;

	Shader & shader = ResourceManager::getShader("shader");
	mat4 model;

	vec2 position_;
	vec2 size_;
	if (screenCoord)
	{
		position_ = vec2(pos) / float(BLOCKSIZE);
		size_ = vec2(size) / float(BLOCKSIZE);
	}
	else
	{
		position_ = vec2(pos);
		size_ = vec2(size);
	}

	shader.use();

	model = translate(model, vec3(position_, 0.0f));
	if (rot != 0.0)
	{
		model = translate(model, vec3((float)size_.x * 0.5f, (float)size_.y * 0.5f, 0.0f));
		model = rotate(model, (float)rot, vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(-(float)size_.x * 0.5f, -(float)size_.y * 0.5f, 0.0f));
	}
	model = scale(model, vec3(vec2(size_), 1.0f));

	shader.set4x4f("model", model);

	glBindTexture(GL_TEXTURE_2D, tex);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::setSize(const dvec2 & v)
{
	size = v;
}

void Renderer::setRotation(double v)
{
	rot = v;
}

void Renderer::setTexture(const std::string & name)
{
	tex = ResourceManager::getTexture(name).getId();
}

void Renderer::setTexture(GLuint v)
{
	tex = v;
}

void Renderer::setScreenCoord(bool v)
{
	screenCoord = v;
}