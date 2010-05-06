/*
 * HeroOnGroundRight.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "HeroRunningRight.h"
#include "HeroStandingRight.h"
#include "Sprite.h"
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
		// The first frame is displayed only once when there is
		// a state transition
		instance->setLoopFirstFrame(1);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void HeroRunningRight::stop(Sprite* sprite) {
	sprite->setState(HeroStandingRight::getInstance());
}

void HeroRunningRight::initialize(Sprite* sprite) {
	sprite->setVelocity(4, 0);
}

void HeroRunningRight::update(Sprite* sprite) {
	// Call base class function
	HeroState::update(sprite);

	// Do some more reset stuff specific to the subclass

}
