/*
 * Enemy.cpp
 *
 *  Created on: May 12, 2010
 *      Author: fgervais
 */

#include "Metool.h"
#include "MetoolState.h"
#include "Environment.h"

Metool::Metool(MetoolState* initialState, Environment* environment) : Sprite(initialState, environment) {
	this->state = initialState;

	// Unsafe?
	this->state->initialize(this);
}

Metool::~Metool() {

}

void Metool::setState(MetoolState* state)  {
	this->state = state;

	// Do state entry initialization on the sprite
	state->initialize(this);

	// Send state to the superclass
	Sprite::setState(state);
}

void Metool::update() {
	environment->move(this, positionX+velocityX, positionY+velocityY);

	// Update the currently displayed frame
	// And possibly some state specific things
	state->update(this);
}

void Metool::collideWith(Collider* collider) {
	collider->collideWith(this);
}
