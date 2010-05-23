/*
 * MegamanOnGroundLeft.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "MegamanRunningLeft.h"
#include "MegamanJumpingLeft.h"
#include "MegamanRunningRight.h"
#include "MegamanStandingLeft.h"
#include "Bitmap.h"
#include "Megaman.h"
#include "Physics.h"
#include "Environment.h"

MegamanState* MegamanRunningLeft::instance = 0;

MegamanRunningLeft::MegamanRunningLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationanimationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
: MegamanState(animationWidth, animationHeight, animationanimationFrames, numberOfFrame, animationMasks) {


}

MegamanRunningLeft::~MegamanRunningLeft() {

}

MegamanState* MegamanRunningLeft::getInstance() {
	if(instance == 0) {
		Bitmap** animationFrames = new Bitmap*[11];
		animationFrames[0] = new Bitmap("0:state/MegamanRunningLeft/1.bmp");
		animationFrames[1] = new Bitmap("0:state/MegamanRunningLeft/2.bmp");
		animationFrames[2] = new Bitmap("0:state/MegamanRunningLeft/3.bmp");
		animationFrames[3] = new Bitmap("0:state/MegamanRunningLeft/4.bmp");
		animationFrames[4] = new Bitmap("0:state/MegamanRunningLeft/5.bmp");
		animationFrames[5] = new Bitmap("0:state/MegamanRunningLeft/6.bmp");
		animationFrames[6] = new Bitmap("0:state/MegamanRunningLeft/7.bmp");
		animationFrames[7] = new Bitmap("0:state/MegamanRunningLeft/8.bmp");
		animationFrames[8] = new Bitmap("0:state/MegamanRunningLeft/9.bmp");
		animationFrames[9] = new Bitmap("0:state/MegamanRunningLeft/10.bmp");
		animationFrames[10] = new Bitmap("0:state/MegamanRunningLeft/11.bmp");

		Bitmap** animationMasks = new Bitmap*[11];
		animationMasks[0] = new Bitmap("0:state/MegamanRunningLeft/mask1.bmp");
		animationMasks[1] = new Bitmap("0:state/MegamanRunningLeft/mask2.bmp");
		animationMasks[2] = new Bitmap("0:state/MegamanRunningLeft/mask3.bmp");
		animationMasks[3] = new Bitmap("0:state/MegamanRunningLeft/mask4.bmp");
		animationMasks[4] = new Bitmap("0:state/MegamanRunningLeft/mask5.bmp");
		animationMasks[5] = new Bitmap("0:state/MegamanRunningLeft/mask6.bmp");
		animationMasks[6] = new Bitmap("0:state/MegamanRunningLeft/mask7.bmp");
		animationMasks[7] = new Bitmap("0:state/MegamanRunningLeft/mask8.bmp");
		animationMasks[8] = new Bitmap("0:state/MegamanRunningLeft/mask9.bmp");
		animationMasks[9] = new Bitmap("0:state/MegamanRunningLeft/mask10.bmp");
		animationMasks[10] = new Bitmap("0:state/MegamanRunningLeft/mask11.bmp");
		instance = new MegamanRunningLeft(35, 35, animationFrames, 11, animationMasks);
	}
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void MegamanRunningLeft::jump(Megaman* sprite) {
	sprite->setVelocityY(-8);
	sprite->setState(MegamanJumpingLeft::getInstance());
}

void MegamanRunningLeft::runRight(Megaman* sprite) {
	sprite->setState(MegamanRunningRight::getInstance());
}

void MegamanRunningLeft::stopRunning(Megaman* sprite) {
	sprite->setState(MegamanStandingLeft::getInstance());
}

void MegamanRunningLeft::initialize(Megaman* sprite) {
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

void MegamanRunningLeft::update(Megaman* sprite) {
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
		sprite->setState(MegamanJumpingLeft::getInstance());
	}

}
