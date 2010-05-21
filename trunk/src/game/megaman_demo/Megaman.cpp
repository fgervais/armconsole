/*
 * Megaman.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Megaman.h"
#include "Environment.h"
#include "MegamanState.h"
#include "Debug.h"
#include "LPC2478.h"

Megaman::Megaman(MegamanState* initialState, Environment* environment) : Sprite(initialState, environment) {
	this->state = initialState;

	// Unsafe?
	this->state->initialize(this);
}

Megaman::~Megaman() {

}

void Megaman::setState(MegamanState* state)  {
	this->state = state;

	// Do state entry initialization on the sprite
	state->initialize(this);

	// Send state to the superclass
	Sprite::setState(state);
}

void Megaman::update() {
	environment->move(this, positionX+velocityX, positionY+velocityY);

	if(collisionCheckEnabled) {
		environment->checkCollision(this);
	}

	// Update the currently displayed frame
	// And possibly some state specific things
	state->update(this);
}

void Megaman::collideWith(Collider* collider) {
	collider->collideWith(this);
}

void Megaman::collideWith(Megaman*) {
	Debug::writeLine("Megaman collided with Megaman");
	LPC2478::delay(1000000);
}

void Megaman::collideWith(Metool*) {
	Debug::writeLine("Megaman collided with Metool");
	LPC2478::delay(1000000);
}

/**
 * This function delegates the action to the current state.
 */
void Megaman::jump() {
	state->jump(this);
}

/**
 * This function delegates the action to the current state.
 */
void Megaman::runLeft() {
	state->runLeft(this);
}

/**
 * This function delegates the action to the current state.
 */
void Megaman::runRight() {
	state->runRight(this);
}

/**
 * This function delegates the action to the current state.
 */
void Megaman::stopRunning() {
	state->stopRunning(this);
}

/**
 * This function delegates the action to the current state.
 */
void Megaman::stopJumping() {
	state->stopJumping(this);
}
