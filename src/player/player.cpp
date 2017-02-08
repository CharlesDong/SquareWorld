#include "player.h"
#include "main/game.h"
#include "renderer/renderer.h"

const dvec2 Player::PlayerPosOnScreen = dvec2(4.5, 4.0);

Player::Player()
{
	mPosition = dvec2(4.5, 16.0);
	mIsJumping = false;
}

void Player::update(double dt, const World & world)
{
	// left/right move
	if (App.isKeyDown(GLFW_KEY_A))
		mVelocity.x = -2.5;
	else if (App.isKeyDown(GLFW_KEY_D))
		mVelocity.x = 2.5;
	else
		mVelocity.x = 0.0;

	// jump (press Space or W to jump)
	if (mIsJumping)
		mVelocity.y -= 5.0 * dt;
	if ((App.isKeyDownNotProc(GLFW_KEY_SPACE) || App.isKeyDownNotProc(GLFW_KEY_W)) && !mIsJumping)
	{
		App.setKeyProc(GLFW_KEY_SPACE);
		App.setKeyProc(GLFW_KEY_W);
		mIsJumping = true;
		mVelocity = dvec2(0.0, 5.0);
	}

	dvec2 posDelta = mVelocity * dt;

	// hit test
	std::vector<AABB> hitboxes = world.getHitboxes(getHitbox().expand(posDelta));

	// hit test on x-axis
	for (AABB box : hitboxes)
		posDelta.x = getHitbox().maxMoveX(box, posDelta.x);
	if (posDelta.x != (mVelocity * dt).x)	// hit other blocks?
		mVelocity.x = 0.0;
	// hit test on y-axis
	for (AABB box : hitboxes)
		posDelta.y = getHitbox().maxMoveY(box, posDelta.y);
	if (posDelta.y != (mVelocity * dt).y)	// hit other blocks?
	{
		if (mIsJumping)
		{
			if (mVelocity.y < 0.0)			// hit ground
			{
				mVelocity.y = 0.0;
				mIsJumping = false;
			}
			else
				mVelocity.y *= -1.0;		// hit block(s) upon the player, inverse the Y velocity
		}
	}
	
	// update position
	mPosition += posDelta;
}

const dvec2 & Player::getPosition() const
{
	return mPosition;
}

dvec2 Player::getSceneOffset() const
{
	return mPosition - PlayerPosOnScreen;
}

AABB Player::getHitbox() const
{
	return AABB(mPosition.x, mPosition.y, mPosition.x + 0.6, mPosition.y + 1.6);
}

bool Player::isJumping() const
{
	return mIsJumping;
}

void Player::render() const
{
	// draw leg
	Renderer::setTexture("legR");
	Renderer::setSize(dvec2(0.2, 0.6));
	Renderer::render(PlayerPosOnScreen + dvec2(0.2, 0.0));

	// draw body
	Renderer::setTexture("bodyR");
	Renderer::setSize(dvec2(0.2, 0.6));
	Renderer::render(PlayerPosOnScreen + dvec2(0.2, 0.6));

	// draw head
	Renderer::setTexture("headR");
	Renderer::setSize(dvec2(0.4, 0.4));
	Renderer::render(PlayerPosOnScreen + dvec2(0.1, 1.2));
}