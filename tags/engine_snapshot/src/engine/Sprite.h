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
class VideoMemory;

class Sprite {
public:
	Sprite(Environment* environment);
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

	// Must be overridden by the subclasses
	virtual uint32_t getWidth() = 0;
	virtual uint32_t getHeight() = 0;
	virtual void update() = 0;
	virtual void render(VideoMemory*) = 0;

	uint8_t isOnGround();
protected:
	// Velocity is in pixel per frame.
	int32_t velocityX;
	int32_t velocityY;

	uint32_t positionX;
	uint32_t positionY;

	Environment* environment;
};

#endif /* SPRITE_H_ */
