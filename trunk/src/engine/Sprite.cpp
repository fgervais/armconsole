/*
 * Sprite.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Sprite.h"
#include "Environment.h"

Sprite::Sprite(Environment* environment) {
	this->environment = environment;

	// Variable init
	this->positionX = 0;
	this->positionY = 0;
	this->velocityX = 0;
	this->velocityY = 0;
}

Sprite::~Sprite() {

}

void Sprite::setVelocity(int32_t x, int32_t y) {
	velocityX = x;
	velocityY = y;
}

void Sprite::setPosition(uint32_t x, uint32_t y) {
	positionX = x;
	positionY = y;
}

uint8_t Sprite::isOnGround() {
	return environment->isOnGround(this);
}
