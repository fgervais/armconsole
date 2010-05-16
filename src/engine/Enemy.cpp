/*
 * Enemy.cpp
 *
 *  Created on: May 12, 2010
 *      Author: fgervais
 */

#include "Enemy.h"
#include "Environment.h"
#include "EnemyState.h"
#include "VideoMemory.h"

Enemy::Enemy(EnemyState* initialState, Environment* environment) : Sprite(environment) {
	this->state = initialState;

	// Variable init
	this->state->initialize(this);
}

Enemy::~Enemy() {

}

uint32_t Enemy::getWidth() {
	return state->getWidth();
}

uint32_t Enemy::getHeight() {
	return state->getHeight();
}

void Enemy::setState(EnemyState* state)  {
	// State transition position adjustment
	positionY += this->state->getHeight() - state->getHeight();

	this->state = state;
	// Do state entry initialization on the sprite
	state->initialize(this);
}

void Enemy::update() {
	environment->move(this, positionX+velocityX, positionY+velocityY);

	// Update the currently displayed frame
	// And possibly some state specific things
	state->update(this);
}

void Enemy::render(VideoMemory* videoMemory) {
	state->render(this, videoMemory);
}
