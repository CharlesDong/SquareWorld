#include "select_frame.h"

#include "common/common.h"
#include "common/resource_manager.h"
#include "main/game.h"
#include "renderer/renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

using glm::vec2;
using glm::vec3;
using glm::mat4;

SelectFrame::SelectFrame()
{
	vao = 0;
	mSelBlock = ivec2(-1, -1);
	mSelBlockPrev = ivec2(-1, -1);
	mLife = 1.0;
}

SelectFrame::~SelectFrame()
{
	glDeleteVertexArrays(1, &vao);
}

void SelectFrame::init()
{
	GLfloat vertices[] =
	{
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mat4 projection = glm::ortho(0.0f, 800.0f / (float)BLOCKSIZE, 0.0f, 600.0f / (float)BLOCKSIZE);
	Shader & shader = ResourceManager::getShader("cursor");
	shader.use();
	shader.set4x4f("projection", projection);
}

void SelectFrame::update(double dt, const dvec2 & off, World & world)
{
	mSelBlockPrev = mSelBlock;
	
	dvec2 pos = off + dvec2(App.getCursorPos()) / BLOCKSIZE;
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);
	mSelBlock = ivec2(pos);

	mOffset = off;

	// destroy the block
	if (mSelBlock == mSelBlockPrev && world.getBlock(mSelBlock) != 0 && App.isMouseDown(0))
		mLife -= dt;
	else
		mLife = 1.0;
	if (mLife <= 0.0)
	{
		world.setBlock(mSelBlock, 0);	// replace the block with air
		mLife = 1.0;
	}

	// place the block (now stone only)
	if (App.isMouseDown(1) && world.getBlock(mSelBlock) == 0)
		world.setBlock(mSelBlock, 3);
}

void SelectFrame::render() const
{
	using glm::translate;

	vec2 pos = vec2(dvec2(mSelBlock) - mOffset);

	Shader & shader = ResourceManager::getShader("cursor");
	mat4 model;
	model = translate(model, vec3(pos, 0.0f));

	shader.use();
	shader.set4x4f("model", model);

	glLineWidth(2.0f);			// make the lines wider

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 8);
	glBindVertexArray(0);

	glLineWidth(1.0f);			// reset the line width

	if (mLife < 1.0)
	{
		int stage = 9 - int(mLife / 0.1);
		std::ostringstream ostr;
		ostr << "destroy" << stage;

		Renderer::setSize(vec2(1.0, 1.0));
		Renderer::setTexture(ostr.str());
		Renderer::render(pos);
	}
}