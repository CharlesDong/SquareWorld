#pragma once

#include "common/common.h"

class AABB;
class World;

class Player
{
private:
	dvec2 mPosition;
	dvec2 mVelocity;
	bool mIsJumping;

	static const dvec2 PlayerPosOnScreen;
public:
	Player();

	const dvec2 & getPosition() const;
	dvec2 getSceneOffset() const;
	AABB getHitbox() const;
	bool isJumping() const;

	void update(double dt, const World & world);
	void render() const;
};