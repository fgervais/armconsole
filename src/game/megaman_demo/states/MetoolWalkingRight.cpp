/*
 * MetoolWalkingRight.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "MetoolWalkingRight.h"
#include "Bitmap.h"
#include "Metool.h"
#include "Environment.h"
#include "Physics.h"

//MetoolState* MetoolWalkingRight::instance = 0;
Bitmap** MetoolWalkingRight::sharedFrames = 0;
Bitmap** MetoolWalkingRight::sharedMasks = 0;

MetoolWalkingRight::MetoolWalkingRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
: MetoolState(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {

}

MetoolWalkingRight::~MetoolWalkingRight() {

}

MetoolState* MetoolWalkingRight::createInstance() {
	//if(instance == 0) {
	if(sharedFrames == 0) {
		//Bitmap** frames = new Bitmap*[4];
		sharedFrames = new Bitmap*[4];
		sharedFrames[0] = new Bitmap("0:state/MetoolWalkingRight/1.bmp");
		sharedFrames[1] = new Bitmap("0:state/MetoolWalkingRight/2.bmp");
		sharedFrames[2] = new Bitmap("0:state/MetoolWalkingRight/3.bmp");
		sharedFrames[3] = new Bitmap("0:state/MetoolWalkingRight/4.bmp");
		//instance = new MetoolWalkingRight(22, 21, frames, 4);
	}
	if(sharedMasks == 0) {
		sharedMasks = new Bitmap*[4];
		sharedMasks[0] = new Bitmap("0:state/MetoolWalkingRight/mask1.bmp");
		sharedMasks[1] = new Bitmap("0:state/MetoolWalkingRight/mask2.bmp");
		sharedMasks[2] = new Bitmap("0:state/MetoolWalkingRight/mask3.bmp");
		sharedMasks[3] = new Bitmap("0:state/MetoolWalkingRight/mask4.bmp");
	}
	MetoolWalkingRight* instance = new MetoolWalkingRight(22, 21, sharedFrames, 4, sharedMasks);
	//instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void MetoolWalkingRight::initialize(Metool* sprite) {
	// This is the hard coded running speed
	// could be (should be?) somewhere defined else?
	sprite->setVelocity(4, 0);
}

void MetoolWalkingRight::update(Metool* sprite) {
	// Update the current frame
	if(currentFrame < (numberOfFrame-1)) {
		currentFrame++;
	}
	else {
		currentFrame = 0;
	}

	// If we loose contact with the ground, then we are falling
	if(!sprite->isOnGround()) {
		// Update speed according to gravity
		int32_t velocityY = sprite->getVelocityY() + sprite->getEnvironment()->getPhysics()->getGravitation();
		sprite->setVelocityY(velocityY);
	}
	else {
		sprite->setVelocityY(0);
	}

}
