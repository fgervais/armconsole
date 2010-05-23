/*
 * MegamanStandingRight.cpp
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#include "MegamanStandingRight.h"
#include "MegamanRunningRight.h"
#include "MegamanRunningLeft.h"
#include "MegamanJumpingRight.h"
#include "Sprite.h"
#include "Megaman.h"
#include "Bitmap.h"

MegamanState* MegamanStandingRight::instance = 0;

MegamanStandingRight::MegamanStandingRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
	: MegamanState(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {


}

MegamanStandingRight::~MegamanStandingRight() {

}

MegamanState* MegamanStandingRight::getInstance() {
	if(instance == 0) {
		Bitmap** animationFrames = new Bitmap*[1];
		animationFrames[0] = new Bitmap("0:state/MegamanStandingRight/1.bmp");

		Bitmap** animationMasks = new Bitmap*[1];
		animationMasks[0] = new Bitmap("0:state/MegamanStandingRight/mask1.bmp");
		instance = new MegamanStandingRight(35, 35, animationFrames, 1, animationMasks);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void MegamanStandingRight::jump(Megaman* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(MegamanJumpingRight::getInstance());
}

void MegamanStandingRight::runLeft(Megaman* sprite) {
	sprite->setState(MegamanRunningLeft::getInstance());
}

void MegamanStandingRight::runRight(Megaman* sprite) {
	sprite->setState(MegamanRunningRight::getInstance());
}

void MegamanStandingRight::update(Megaman* sprite) {
	// This should happen only if the Megaman spawn in the air
	if(!sprite->isOnGround()) {
		sprite->setState(MegamanJumpingRight::getInstance());
	}
}

void MegamanStandingRight::initialize(Megaman* sprite) {
	sprite->setVelocity(0, 0);
}
