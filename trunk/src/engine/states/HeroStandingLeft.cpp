/*
 * HeroStandingLeft.cpp
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#include "HeroStandingLeft.h"
#include "HeroJumpingLeft.h"
#include "HeroRunningRight.h"
#include "HeroRunningLeft.h"
#include "Bitmap.h"
#include "Hero.h"

HeroState* HeroStandingLeft::instance = 0;

HeroStandingLeft::HeroStandingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: HeroState(width, height, frames, numberOfFrame) {


}

HeroStandingLeft::~HeroStandingLeft() {

}

HeroState* HeroStandingLeft::getInstance() {
	if(instance == 0) {
		Bitmap** frames = new Bitmap*[1];
		frames[0] = new Bitmap("0:state/HeroStandingLeft/1.bmp");
		instance = new HeroStandingLeft(35, 35, frames, 1);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void HeroStandingLeft::jump(Hero* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(HeroJumpingLeft::getInstance());
}

void HeroStandingLeft::runLeft(Hero* sprite) {
	sprite->setState(HeroRunningLeft::getInstance());
}

void HeroStandingLeft::runRight(Hero* sprite) {
	sprite->setState(HeroRunningRight::getInstance());
}

void HeroStandingLeft::update(Hero* sprite) {
	// This should happen only if the hero spawn in the air
	if(!sprite->isOnGround()) {
		sprite->setState(HeroJumpingLeft::getInstance());
	}
}

void HeroStandingLeft::initialize(Hero* sprite) {
	sprite->setVelocity(0, 0);
}
