/*
 * MegamanJumpingLeft.cpp
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#include "MegamanJumpingLeft.h"
#include "MegamanStandingLeft.h"
#include "MegamanJumpingRight.h"
#include "MegamanRunningLeft.h"
#include "Bitmap.h"
#include "Megaman.h"
#include "Physics.h"
#include "Environment.h"

MegamanState* MegamanJumpingLeft::instance = 0;

MegamanJumpingLeft::MegamanJumpingLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame)
	: MegamanState(animationWidth, animationHeight, animationFrames, numberOfFrame) {

}

MegamanJumpingLeft::~MegamanJumpingLeft() {

}

MegamanState* MegamanJumpingLeft::getInstance() {
	if(instance == 0) {
		Bitmap** animationFrames = new Bitmap*[7];
		animationFrames[0] = new Bitmap("0:state/HeroJumpingLeft/1.bmp");
		animationFrames[1] = new Bitmap("0:state/HeroJumpingLeft/2.bmp");
		animationFrames[2] = new Bitmap("0:state/HeroJumpingLeft/3.bmp");
		animationFrames[3] = new Bitmap("0:state/HeroJumpingLeft/4.bmp");
		animationFrames[4] = new Bitmap("0:state/HeroJumpingLeft/5.bmp");
		animationFrames[5] = new Bitmap("0:state/HeroJumpingLeft/6.bmp");
		animationFrames[6] = new Bitmap("0:state/HeroJumpingLeft/7.bmp");
		instance = new MegamanJumpingLeft(30, 46, animationFrames, 7);
	}
	instance->reset();
	return instance;
}

void MegamanJumpingLeft::runLeft(Megaman* sprite) {
	sprite->setVelocityX(-6);
}

void MegamanJumpingLeft::runRight(Megaman* sprite) {
	sprite->setVelocityX(6);
	sprite->setState(MegamanJumpingRight::getInstance());
}

void MegamanJumpingLeft::stopRunning(Megaman* sprite) {
	sprite->setVelocityX(0);
}

void MegamanJumpingLeft::stopJumping(Megaman* sprite) {
	sprite->setVelocityY(0);
}

void MegamanJumpingLeft::initialize(Megaman* sprite) {
	if(sprite->getVelocityY() >= 0) {
		// If we are falling, we should start rendering at frame 3
		currentFrame = 3;
	}
}

void MegamanJumpingLeft::update(Megaman* sprite) {
	if(sprite->isOnGround()) {
		// We need to display the landing frames before switching state
		// but only if we are not running
		if(currentFrame < 6 && sprite->getVelocityX() == 0) {
			currentFrame++;
		}
		else {
			if(sprite->getVelocityX() < 0) {
				sprite->setState(MegamanRunningLeft::getInstance());
			}
			else {
				sprite->setState(MegamanStandingLeft::getInstance());

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
