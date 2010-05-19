/*
 * EnemyWalkingLeft.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "EnemyWalkingLeft.h"
#include "Bitmap.h"
#include "Enemy.h"
#include "Environment.h"
#include "Physics.h"

//EnemyState* EnemyWalkingLeft::instance = 0;
Bitmap** EnemyWalkingLeft::sharedFrames = 0;

EnemyWalkingLeft::EnemyWalkingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: EnemyState(width, height, frames, numberOfFrame) {

}

EnemyWalkingLeft::~EnemyWalkingLeft() {

}

EnemyState* EnemyWalkingLeft::getInstance() {
	//if(instance == 0) {
	if(sharedFrames == 0) {
		//Bitmap** frames = new Bitmap*[4];
		sharedFrames = new Bitmap*[4];
		sharedFrames[0] = new Bitmap("0:state/EnemyWalkingLeft/1.bmp");
		sharedFrames[1] = new Bitmap("0:state/EnemyWalkingLeft/2.bmp");
		sharedFrames[2] = new Bitmap("0:state/EnemyWalkingLeft/3.bmp");
		sharedFrames[3] = new Bitmap("0:state/EnemyWalkingLeft/4.bmp");
		//instance = new EnemyWalkingLeft(22, 21, frames, 4);
	}
	EnemyWalkingLeft* instance = new EnemyWalkingLeft(22, 21, sharedFrames, 4);
	instance->reset();
	return instance;
}

/* BASE CLASS FUNCTION OVERRIDE */
void EnemyWalkingLeft::initialize(Enemy* sprite) {
	// This is the hard coded running speed
	// could be (should be?) somewhere defined else?
	sprite->setVelocity(-4, 0);
}

void EnemyWalkingLeft::update(Enemy* sprite) {
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
