/*
 * MegamanOnGroundRight.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "MegamanRunningRight.h"
#include "MegamanStandingRight.h"
#include "MegamanJumpingRight.h"
#include "MegamanRunningLeft.h"
#include "Sprite.h"
#include "Megaman.h"
#include "Bitmap.h"

MegamanState* MegamanRunningRight::instance = 0;

MegamanRunningRight::MegamanRunningRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
	: MegamanState(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {


}

MegamanRunningRight::~MegamanRunningRight() {

}

MegamanState* MegamanRunningRight::getInstance() {
	if(instance == 0) {
		Bitmap** animationFrames = new Bitmap*[11];
		animationFrames[0] = new Bitmap("0:state/MegamanRunningRight/1.bmp");
		animationFrames[1] = new Bitmap("0:state/MegamanRunningRight/2.bmp");
		animationFrames[2] = new Bitmap("0:state/MegamanRunningRight/3.bmp");
		animationFrames[3] = new Bitmap("0:state/MegamanRunningRight/4.bmp");
		animationFrames[4] = new Bitmap("0:state/MegamanRunningRight/5.bmp");
		animationFrames[5] = new Bitmap("0:state/MegamanRunningRight/6.bmp");
		animationFrames[6] = new Bitmap("0:state/MegamanRunningRight/7.bmp");
		animationFrames[7] = new Bitmap("0:state/MegamanRunningRight/8.bmp");
		animationFrames[8] = new Bitmap("0:state/MegamanRunningRight/9.bmp");
		animationFrames[9] = new Bitmap("0:state/MegamanRunningRight/10.bmp");
		animationFrames[10] = new Bitmap("0:state/MegamanRunningRight/11.bmp");

		Bitmap** animationMasks = new Bitmap*[11];
		animationMasks[0] = new Bitmap("0:state/MegamanRunningRight/mask1.bmp");
		animationMasks[1] = new Bitmap("0:state/MegamanRunningRight/mask2.bmp");
		animationMasks[2] = new Bitmap("0:state/MegamanRunningRight/mask3.bmp");
		animationMasks[3] = new Bitmap("0:state/MegamanRunningRight/mask4.bmp");
		animationMasks[4] = new Bitmap("0:state/MegamanRunningRight/mask5.bmp");
		animationMasks[5] = new Bitmap("0:state/MegamanRunningRight/mask6.bmp");
		animationMasks[6] = new Bitmap("0:state/MegamanRunningRight/mask7.bmp");
		animationMasks[7] = new Bitmap("0:state/MegamanRunningRight/mask8.bmp");
		animationMasks[8] = new Bitmap("0:state/MegamanRunningRight/mask9.bmp");
		animationMasks[9] = new Bitmap("0:state/MegamanRunningRight/mask10.bmp");
		animationMasks[10] = new Bitmap("0:state/MegamanRunningRight/mask11.bmp");
		instance = new MegamanRunningRight(35, 35, animationFrames, 11, animationMasks);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void MegamanRunningRight::jump(Megaman* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(MegamanJumpingRight::getInstance());
}

void MegamanRunningRight::runLeft(Megaman* sprite) {
	sprite->setState(MegamanRunningLeft::getInstance());
}

void MegamanRunningRight::stopRunning(Megaman* sprite) {
	sprite->setState(MegamanStandingRight::getInstance());
}

void MegamanRunningRight::initialize(Megaman* sprite) {
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

void MegamanRunningRight::update(Megaman* sprite) {
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
		sprite->setState(MegamanJumpingRight::getInstance());
	}

}
