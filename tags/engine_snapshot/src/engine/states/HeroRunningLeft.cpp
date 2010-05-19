/*
 * HeroOnGroundLeft.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "HeroRunningLeft.h"
#include "HeroJumpingLeft.h"
#include "HeroRunningRight.h"
#include "HeroStandingLeft.h"
#include "Bitmap.h"
#include "Hero.h"
#include "Physics.h"
#include "Environment.h"

HeroState* HeroRunningLeft::instance = 0;

HeroRunningLeft::HeroRunningLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: HeroState(width, height, frames, numberOfFrame) {


}

HeroRunningLeft::~HeroRunningLeft() {

}

HeroState* HeroRunningLeft::getInstance() {
	if(instance == 0) {
		Bitmap** frames = new Bitmap*[11];
		frames[0] = new Bitmap("0:state/HeroRunningLeft/1.bmp");
		frames[1] = new Bitmap("0:state/HeroRunningLeft/2.bmp");
		frames[2] = new Bitmap("0:state/HeroRunningLeft/3.bmp");
		frames[3] = new Bitmap("0:state/HeroRunningLeft/4.bmp");
		frames[4] = new Bitmap("0:state/HeroRunningLeft/5.bmp");
		frames[5] = new Bitmap("0:state/HeroRunningLeft/6.bmp");
		frames[6] = new Bitmap("0:state/HeroRunningLeft/7.bmp");
		frames[7] = new Bitmap("0:state/HeroRunningLeft/8.bmp");
		frames[8] = new Bitmap("0:state/HeroRunningLeft/9.bmp");
		frames[9] = new Bitmap("0:state/HeroRunningLeft/10.bmp");
		frames[10] = new Bitmap("0:state/HeroRunningLeft/11.bmp");
		instance = new HeroRunningLeft(35, 35, frames, 11);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void HeroRunningLeft::jump(Hero* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(HeroJumpingLeft::getInstance());
}

void HeroRunningLeft::runRight(Hero* sprite) {
	sprite->setState(HeroRunningRight::getInstance());
}

void HeroRunningLeft::stopRunning(Hero* sprite) {
	sprite->setState(HeroStandingLeft::getInstance());
}

void HeroRunningLeft::initialize(Hero* sprite) {
	if(sprite->getVelocityX() < 0) {
		// If we are already moving left, skip the first frame
		currentFrame = 1;
	}
	else {
		// This is the hard coded running speed
		// could be (should be?) somewhere defined else?
		sprite->setVelocity(-6, 0);
	}
}

void HeroRunningLeft::update(Hero* sprite) {
	// Update current frame
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
		sprite->setState(HeroJumpingLeft::getInstance());
	}

}
