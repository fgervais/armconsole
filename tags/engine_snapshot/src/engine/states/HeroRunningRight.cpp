/*
 * HeroOnGroundRight.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "HeroRunningRight.h"
#include "HeroStandingRight.h"
#include "HeroJumpingRight.h"
#include "HeroRunningLeft.h"
#include "Sprite.h"
#include "Hero.h"
#include "Bitmap.h"

HeroState* HeroRunningRight::instance = 0;

HeroRunningRight::HeroRunningRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
	: HeroState(width, height, frames, numberOfFrame) {


}

HeroRunningRight::~HeroRunningRight() {

}

HeroState* HeroRunningRight::getInstance() {
	if(instance == 0) {
		Bitmap** frames = new Bitmap*[11];
		frames[0] = new Bitmap("0:state/HeroRunningRight/1.bmp");
		frames[1] = new Bitmap("0:state/HeroRunningRight/2.bmp");
		frames[2] = new Bitmap("0:state/HeroRunningRight/3.bmp");
		frames[3] = new Bitmap("0:state/HeroRunningRight/4.bmp");
		frames[4] = new Bitmap("0:state/HeroRunningRight/5.bmp");
		frames[5] = new Bitmap("0:state/HeroRunningRight/6.bmp");
		frames[6] = new Bitmap("0:state/HeroRunningRight/7.bmp");
		frames[7] = new Bitmap("0:state/HeroRunningRight/8.bmp");
		frames[8] = new Bitmap("0:state/HeroRunningRight/9.bmp");
		frames[9] = new Bitmap("0:state/HeroRunningRight/10.bmp");
		frames[10] = new Bitmap("0:state/HeroRunningRight/11.bmp");
		instance = new HeroRunningRight(35, 35, frames, 11);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void HeroRunningRight::jump(Hero* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(HeroJumpingRight::getInstance());
}

void HeroRunningRight::runLeft(Hero* sprite) {
	sprite->setState(HeroRunningLeft::getInstance());
}

void HeroRunningRight::stopRunning(Hero* sprite) {
	sprite->setState(HeroStandingRight::getInstance());
}

void HeroRunningRight::initialize(Hero* sprite) {
	if(sprite->getVelocityX() > 0) {
		// If we are already moving right, skip the first frame
		currentFrame = 1;
	}
	else {
		// This is the hard coded running speed
		// could be (should be?) somewhere defined else?
		sprite->setVelocity(6, 0);
	}
}

void HeroRunningRight::update(Hero* sprite) {
	// Update the current frame
	if(currentFrame < (numberOfFrame-1)) {
		currentFrame++;
	}
	else {
		// The first frame is displayed only once when we transition from
		// a standing position
		currentFrame = 1;
	}

	// If we loose contact with the ground, then we are falling
	// In our case, falling and jumping is handled by the same state
	if(!sprite->isOnGround()) {
		sprite->setState(HeroJumpingRight::getInstance());
	}

}
