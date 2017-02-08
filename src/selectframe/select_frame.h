#pragma once

#include <GL/glew.h>
#include "common/common.h"

class World;

class SelectFrame
{
private:
	GLuint vao;
	ivec2 mSelBlock;			// the selected block position
	ivec2 mSelBlockPrev;		// the selected block position in last frame
	dvec2 mOffset;				// scene offset (the game position of the screen's left-bottom corner)
	double mLife;				// time in secs before a block is destroyed
public:
	SelectFrame();
	~SelectFrame();
	void init();
	void update(double dt, const dvec2 & offset, World & world);
	void render() const;
};