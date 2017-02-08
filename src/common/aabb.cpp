#include "aabb.h"

AABB::AABB(double x0, double y0, double x1, double y1)
{
	min.x = x0;
	min.y = y0;
	max.x = x1;
	max.y = y1;
}

AABB::AABB(const dvec2 & v0, const dvec2 & v1)
{
	min = v0;
	max = v1;
}

// Expand an AABB.
AABB AABB::expand(const dvec2 & v) const
{
	AABB result(*this);
	if (v.x > 0.0)
		result.max.x += v.x;
	else
		result.min.x += v.x;
	if (v.y > 0.0)
		result.max.y += v.y;
	else
		result.min.y += v.y;
	return result;
}

// Maximum move distance on x-axis while not blocked by another AABB.
double AABB::maxMoveX(const AABB & b, double dist)
{
	// no intersect or zero move
	if (!intersectY(*this, b) || dist == 0.0)
		return dist;
	// collide
	if (intersect(*this, b))
		return 0.0;
	// on the left of another AABB and will collide
	if (max.x <= b.min.x && max.x + dist > b.min.x)
		return b.min.x - max.x;
	// on the right of another AABB and will collide
	if (min.x >= b.max.x && min.x + dist < b.max.x)
		return b.max.x - min.x;
	// won't collide
	return dist;
}

// Maximum move distance on y-axis while not blocked by another AABB.
double AABB::maxMoveY(const AABB & b, double dist)
{
	// no intersect or zero move
	if (!intersectX(*this, b) || dist == 0.0)
		return dist;
	// collide
	if (intersect(*this, b))
		return 0.0;
	// under another AABB and will collide
	if (max.y <= b.min.y && max.y + dist > b.min.y)
		return b.min.y - max.y;
	// above another AABB and will collide
	if (min.y >= b.max.y && min.y + dist < b.max.y)
		return b.max.y - min.y;
	// won't collide
	return dist;
}

bool intersectX(const AABB & a, const AABB & b)
{
	return a.max.x > b.min.x && b.max.x > a.min.x;
}

bool intersectY(const AABB & a, const AABB & b)
{
	return a.max.y > b.min.y && b.max.y > a.min.y;
}

bool intersect(const AABB & a, const AABB & b)
{
	return intersectX(a, b) && intersectY(a, b);
}