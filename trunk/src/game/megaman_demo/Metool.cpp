/*
 * Enemy.cpp
 *
 *  Created on: May 12, 2010
 *      Author: fgervais
 */

#include "Metool.h"
#include "MetoolState.h"
#include "Environment.h"
#include "MetoolWalkingLeft.h"
#include "MetoolWalkingRight.h"
#include "Debug.h"
#include "LPC2478.h"

Metool::Metool(MetoolState* initialState, Environment* environment) : Sprite(initialState, environment) {
	// Keep a pointer to the initial state in case we need to re-spawn the sprite
	//this->initialState = initialState;
	this->currentState = initialState;

	// Unsafe?
	this->currentState->initialize(this);

	// Initialize every possible Metool states
	//metoolWalkingLeft = MetoolWalkingLeft::createInstance();
	//metoolWalkingLeft = MetoolWalkingRight::createInstance();
}

Metool::~Metool() {

}

void Metool::setState(MetoolState* state)  {
	this->currentState = state;

	// Do state entry initialization on the sprite
	currentState->initialize(this);

	// Send state to the superclass
	Sprite::setState(state);
}

void Metool::update() {
	environment->move(this, positionX+velocityX, positionY+velocityY);

	// Update the currently displayed frame
	// And possibly some state specific things
	currentState->update(this);
}

void Metool::collideWith(Collider* collider) {
	collider->collideWith(this);
}

void Metool::collideWith(Megaman*) {
	Debug::writeLine("Metool collided with Megaman");
	LPC2478::delay(1000000);
}

void Metool::collideWith(Metool*) {
	Debug::writeLine("Metool collided with Metool");
	LPC2478::delay(1000000);
}
