/*
 * Hero.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Hero.h"
#include "Environment.h"
#include "HeroState.h"
#include "VideoMemory.h"
#include "Debug.h"

Hero::Hero(HeroState* initialState, Environment* environment) : Sprite(environment) {
	this->state = initialState;

	// Variable init
}

Hero::~Hero() {

}

uint32_t Hero::getWidth() {
	return state->getWidth();
}

uint32_t Hero::getHeight() {
	return state->getHeight();
}

void Hero::setState(HeroState* state)  {
	// State transition position adjustment
	positionY += this->state->getHeight() - state->getHeight();

	this->state = state;
	// Do state entry initialization on the sprite
	state->initialize(this);
}

void Hero::update() {
	environment->move(this, positionX+velocityX, positionY+velocityY);

	// Update the currently displayed frame
	// And possibly some state specific things
	state->update(this);
}

void Hero::render(VideoMemory* videoMemory) {
	state->render(this, videoMemory);
}

/**
 * This function delegates the action to the current state.
 */
void Hero::jump() {
	state->jump(this);
}

/**
 * This function delegates the action to the current state.
 */
void Hero::runLeft() {
	state->runLeft(this);
}

/**
 * This function delegates the action to the current state.
 */
void Hero::runRight() {
	state->runRight(this);
}

/**
 * This function delegates the action to the current state.
 */
void Hero::stopRunning() {
	state->stopRunning(this);
}

/**
 * This function delegates the action to the current state.
 */
void Hero::stopJumping() {
	state->stopJumping(this);
}
