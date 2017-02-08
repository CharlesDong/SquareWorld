#include "world.h"
#include "common/common.h"
#include "renderer/renderer.h"
#include "main/game.h"
#include <algorithm>
#include <iostream>

World::World()
{
}

World::~World()
{
}

void World::load()
{
	int i;
	for (i = 0; i < 24; i++)
	{
		for (item_t & b : mData[i])
		{
			if (i == 0)
				b = BLOCK_BEDROCK;
			else if (i >= 1 && i <= 12)
				b = BLOCK_STONE;
			else if (i >= 13 && i <= 14)
				b = BLOCK_DIRT;
			else if (i == 15)
				b = BLOCK_GRASS;
			else
				b = 0;
		}
	}
}

void World::setBlock(const ivec2 & pos, item_t id)
{
	if (pos.x < 0 || pos.x >= 50 || pos.y < 0 || pos.y >= 24)	// out of bounds
		return;
	mData[pos.y][pos.x] = id;
}

item_t World::getBlock(const ivec2 & pos)
{
	if (pos.x < 0 || pos.x >= 50 || pos.y < 0 || pos.y >= 24)	// out of bounds
		return 255;
	return mData[pos.y][pos.x];
}

std::vector<AABB> World::getHitboxes(const AABB & hitbox) const
{
	int x, y;
	std::vector<AABB> result;
	for (y = (int)floor(hitbox.min.y); y <= (int)ceil(hitbox.max.y); y++)
		for (x = (int)floor(hitbox.min.x); x <= (int)ceil(hitbox.max.x); x++)
			if (x >= 0 && x < 50 && y >= 0 && y < 24 && mData[y][x] != 0)
				result.push_back(AABB(double(x), double(y), double(x) + 1.0, double(y) + 1.0));
	return result;
}

void World::render(const dvec2 & off) const
{
	using std::min;
	using std::max;

	int x, y;
	int xMin, xMax;
	int yMin, yMax;
	xMin = max(int(off.x), 0);
	yMin = max(int(off.y), 0);
	xMax = min(int(off.x + App.getWidth() / BLOCKSIZE), 49);
	yMax = min(int(off.y + App.getHeight() / BLOCKSIZE), 23);

	int idPrev = -1;
	Renderer::setSize(dvec2(1.0, 1.0));
	for (y = yMin; y <= yMax; y++)
		for (x = xMin; x <= xMax; x++)
		{
			if (mData[y][x] == 0)	// air block, skip it
				continue;
			if (mData[y][x] != idPrev)	// only switch texture if this block is different from the last one
				Renderer::setTexture(itemName(mData[y][x]));
			Renderer::render(dvec2(x, y) - off);
			idPrev = mData[y][x];
		}
}