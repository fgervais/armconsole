/*
 * EnemyWalkingRight.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "EnemyWalkingRight.h"
#include "Bitmap.h"
#include "Enemy.h"
#include "Environment.h"
#include "Physics.h"

//EnemyState* EnemyWalkingRight::instance = 0;
Bitmap** EnemyWalkingRight::sharedFrames = 0;

EnemyWalkingRight::EnemyWalkingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: EnemyState(width, height, frames, numberOfFrame) {

}

EnemyWalkingRight::~EnemyWalkingRight() {

}

EnemyState* EnemyWalkingRight::getInstance() {
	//if(instance == 0) {
	if(sharedFrames == 0) {
		//Bitmap** frames = new Bitmap*[4];
		sharedFrames = new Bitmap*[4];
		sharedFrames[0] = new Bitmap("0:state/EnemyWalkingRight/1.bmp");
		sharedFrames[1] = new Bitmap("0:state/EnemyWalkingRight/2.bmp");
		sharedFrames[2] = new Bitmap("0:state/EnemyWalkingRight/3.bmp");
		sharedFrames[3] = new Bitmap("0:state/EnemyWalkingRight/4.bmp");
		//instance = new EnemyWalkingRight(22, 21, frames, 4);
	}
	EnemyWalkingRight* instance = new EnemyWalkingRight(22, 21, sharedFrames, 4);
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void EnemyWalkingRight::initialize(Enemy* sprite) {
	// This is the hard coded running speed
	// could be (should be?) somewhere defined else?
	sprite->setVelocity(4, 0);
}

void EnemyWalkingRight::update(Enemy* sprite) {
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
