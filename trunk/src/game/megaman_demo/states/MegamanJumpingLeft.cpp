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
#include "Wave.h"
#include "AudioHelper.h"
#include "Audio.h"
#include "LPC2478.h"

MegamanState* MegamanJumpingLeft::instance = 0;

MegamanJumpingLeft::MegamanJumpingLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
	: MegamanState(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {

}

MegamanJumpingLeft::~MegamanJumpingLeft() {

}

MegamanState* MegamanJumpingLeft::getInstance() {
	if(instance == 0) {
		Bitmap** animationFrames = new Bitmap*[7];
		animationFrames[0] = new Bitmap("0:state/MegamanJumpingLeft/1.bmp");
		animationFrames[1] = new Bitmap("0:state/MegamanJumpingLeft/2.bmp");
		animationFrames[2] = new Bitmap("0:state/MegamanJumpingLeft/3.bmp");
		animationFrames[3] = new Bitmap("0:state/MegamanJumpingLeft/4.bmp");
		animationFrames[4] = new Bitmap("0:state/MegamanJumpingLeft/5.bmp");
		animationFrames[5] = new Bitmap("0:state/MegamanJumpingLeft/6.bmp");
		animationFrames[6] = new Bitmap("0:state/MegamanJumpingLeft/7.bmp");

		Bitmap** animationMasks = new Bitmap*[7];
		animationMasks[0] = new Bitmap("0:state/MegamanJumpingLeft/mask1.bmp");
		animationMasks[1] = new Bitmap("0:state/MegamanJumpingLeft/mask2.bmp");
		animationMasks[2] = new Bitmap("0:state/MegamanJumpingLeft/mask3.bmp");
		animationMasks[3] = new Bitmap("0:state/MegamanJumpingLeft/mask4.bmp");
		animationMasks[4] = new Bitmap("0:state/MegamanJumpingLeft/mask5.bmp");
		animationMasks[5] = new Bitmap("0:state/MegamanJumpingLeft/mask6.bmp");
		animationMasks[6] = new Bitmap("0:state/MegamanJumpingLeft/mask7.bmp");


		instance = new MegamanJumpingLeft(30, 46, animationFrames, 7, animationMasks);
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
	else {
		Audio::playAsync(sprite->getJumpSoundFX());
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
			Audio::playAsync(sprite->getLandSoundFX());
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
