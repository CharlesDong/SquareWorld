#pragma once

#include "common.h"

class AABB
{
public:
	dvec2 min;
	dvec2 max;

	AABB(double x0, double y0, double x1, double y1);
	AABB(const dvec2 & v0, const dvec2 & v1);

	AABB expand(const dvec2 & v) const;
	double maxMoveX(const AABB & b, double dist);
	double maxMoveY(const AABB & b, double dist);
};

bool intersectX(const AABB & a, const AABB & b);
bool intersectY(const AABB & a, const AABB & b);
bool intersect(const AABB & a, const AABB & b);