/*
 * HeroStandingRight.cpp
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#include "HeroStandingRight.h"
#include "HeroRunningRight.h"
#include "Sprite.h"
#include "Bitmap.h"

HeroState* HeroStandingRight::instance = 0;

HeroStandingRight::HeroStandingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
	: HeroState(width, height, frames, numberOfFrame) {


}

HeroStandingRight::~HeroStandingRight() {

}

HeroState* HeroStandingRight::getInstance() {
	if(instance == 0) {
		Bitmap** frames = new Bitmap*[1];
		frames[0] = new Bitmap("0:state/HeroStandingRight/1.bmp");
		instance = new HeroStandingRight(35, 35, frames, 1);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void HeroStandingRight::runRight(Sprite* sprite) {
	sprite->setState(HeroRunningRight::getInstance());
}

void HeroStandingRight::initialize(Sprite* sprite) {
	sprite->setVelocity(0, 0);
}
