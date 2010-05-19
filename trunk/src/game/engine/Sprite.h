/*
 * Sprite.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "Collider.h"

#include <stdint.h>

class Environment;
class VideoMemory;
class State;

class Sprite : public Collider {
public:
	Sprite(State* initialState, Environment* environment);
	virtual ~Sprite();

	// Accessors
	void setVelocity(int32_t x, int32_t y);
	void setVelocityX(int32_t x);
	void setVelocityY(int32_t y);
	int32_t getVelocityX() { return velocityX; };
	int32_t getVelocityY() { return velocityY; };
	Environment* getEnvironment() { return environment; }

	void setPosition(uint32_t x, uint32_t y);
	void setPositionX(uint32_t x);
	void setPositionY(uint32_t y);
	uint32_t getPositionX() { return positionX; }
	uint32_t getPositionY() { return positionY; }

	uint32_t getWidth();
	uint32_t getHeight();

	void setState(State* state);

	virtual void update() =0;
	virtual void render(VideoMemory*);

	uint8_t isOnGround();

	// Collision functions
	void enableCollisionCheck() { collisionCheckEnabled = 1; }
	void disableCollisionCheck() { collisionCheckEnabled = 0; }
protected:
	// Velocity is in pixel per frame.
	int32_t velocityX;
	int32_t velocityY;

	uint32_t positionX;
	uint32_t positionY;

	Environment* environment;

	// Flags
	uint8_t collisionCheckEnabled;
private:
	State* state;
};

#endif /* SPRITE_H_ */
