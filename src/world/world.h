#pragma once

#include <vector>
#include "common/common.h"
#include "common/aabb.h"

class World
{
private:
	item_t mData[24][50];
public:
	World();
	~World();
	void load();
	void setBlock(const ivec2 & pos, item_t id);
	item_t getBlock(const ivec2 & pos);
	std::vector<AABB> getHitboxes(const AABB & hitbox) const;
	void render(const dvec2 & offset) const;
};