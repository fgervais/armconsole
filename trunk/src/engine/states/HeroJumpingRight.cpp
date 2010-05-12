/*
 * HeroJumpingRight.cpp
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#include "HeroJumpingRight.h"
#include "HeroStandingRight.h"
#include "HeroJumpingLeft.h"
#include "HeroRunningRight.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Hero.h"
#include "Physics.h"
#include "Environment.h"
#include "Debug.h"

HeroState* HeroJumpingRight::instance = 0;

HeroJumpingRight::HeroJumpingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
	: HeroState(width, height, frames, numberOfFrame) {


}

HeroJumpingRight::~HeroJumpingRight() {

}

HeroState* HeroJumpingRight::getInstance() {
	if(instance == 0) {
		Bitmap** frames = new Bitmap*[7];
		frames[0] = new Bitmap("0:state/HeroJumpingRight/1.bmp");
		frames[1] = new Bitmap("0:state/HeroJumpingRight/2.bmp");
		frames[2] = new Bitmap("0:state/HeroJumpingRight/3.bmp");
		frames[3] = new Bitmap("0:state/HeroJumpingRight/4.bmp");
		frames[4] = new Bitmap("0:state/HeroJumpingRight/5.bmp");
		frames[5] = new Bitmap("0:state/HeroJumpingRight/6.bmp");
		frames[6] = new Bitmap("0:state/HeroJumpingRight/7.bmp");
		instance = new HeroJumpingRight(30, 46, frames, 7);
	}
	instance->reset();
	return instance;
}

void HeroJumpingRight::runLeft(Hero* sprite) {
	sprite->setVelocityX(-6);
	sprite->setState(HeroJumpingLeft::getInstance());
}

void HeroJumpingRight::runRight(Hero* sprite) {
	sprite->setVelocityX(6);
}

void HeroJumpingRight::stopRunning(Hero* sprite) {
	sprite->setVelocityX(0);
}

void HeroJumpingRight::stopJumping(Hero* sprite) {
	sprite->setVelocityY(0);
}

void HeroJumpingRight::initialize(Hero* sprite) {
	if(sprite->getVelocityY() >= 0) {
		// If we are falling, we should start rendering at frame 3
		currentFrame = 3;
	}
}

void HeroJumpingRight::update(Hero* sprite) {
	if(sprite->isOnGround()) {
		// We need to display the landing frames before switching state
		// but only if we are not running
		if(currentFrame < 6 && sprite->getVelocityX() == 0) {
			currentFrame++;
		}
		else {
			if(sprite->getVelocityX() > 0) {
				sprite->setState(HeroRunningRight::getInstance());
			}
			else {
				sprite->setState(HeroStandingRight::getInstance());

			}
		}
		//sprite->setVelocity(sprite->getVelocityX(), 0);
	}
	else {
		// Manual frame update
		// Going up
		if(sprite->getVelocityY() < 0) {
			if(currentFrame < 2) {
				currentFrame++;
			}
		}
		// Going down
		else {
			if(currentFrame < 4) {
				currentFrame++;
			}
		}
		// Update speed according to gravity
		int32_t velocityY = sprite->getVelocityY() + sprite->getEnvironment()->getPhysics()->getGravitation();
		sprite->setVelocityY(velocityY);

	}
}
