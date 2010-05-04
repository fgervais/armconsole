/*
 * Sprite.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Sprite.h"
#include "Environment.h"
#include "HeroState.h"
#include "VideoMemory.h"

Sprite::Sprite(uint32_t width, uint32_t height, HeroState* initialState, Environment* environment) {
	this->width = width;
	this->height = height;
	this->state = initialState;
	this->environment = environment;

	// Variable init
	this->positionX = 0;
	this->positionY = 0;
	this->velocityX = 0;
	this->velocityY = 0;
}

Sprite::~Sprite() {

}

void Sprite::setVelocity(uint32_t x, uint32_t y) {
	velocityX = x;
	velocityY = y;
}

void Sprite::setPosition(uint32_t x, uint32_t y) {
	positionX = x;
	positionY = y;
}

void Sprite::update() {
	state->update(this);
}

void Sprite::render(VideoMemory* videoMemory) {
	state->render(this, videoMemory);
}
