/*
 * Sprite.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdint.h>

class Environment;
class State;

class Sprite {
public:
	Sprite(uint32_t width, uint32_t height, State* initialState, Environment* environment);
	virtual ~Sprite();

	enum Direction { Up, Down, Left, Right };

	void set(Direction direction) { this->direction = direction; }

	int32_t getVelocityX() { return xVelocity; };
	int32_t getVelocityY() { return yVelocity; };
	Direction getDirection() { return direction; }
private:
	Direction direction;

	// For now the velocity is in pixel per frame.
	int32_t xVelocity;
	int32_t yVelocity;
};

#endif /* SPRITE_H_ */
