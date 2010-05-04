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
class HeroState;
class VideoMemory;

class Sprite {
public:
	Sprite(uint32_t width, uint32_t height, HeroState* initialState, Environment* environment);
	virtual ~Sprite();

	// Accessors
	void setVelocity(uint32_t x, uint32_t y);
	int32_t getVelocityX() { return velocityX; };
	int32_t getVelocityY() { return velocityY; };
	Environment* getEnvironment() { return environment; }

	void setPosition(uint32_t x, uint32_t y);
	uint32_t getPositionX() { return positionX; }
	uint32_t getPositionY() { return positionY; }

	uint32_t getWidth() { return width; };
	uint32_t getHeight() { return height; };

	virtual void update();
	virtual void render(VideoMemory*);
private:
	// For now the velocity is in pixel per frame.
	int32_t velocityX;
	int32_t velocityY;

	uint32_t positionX;
	uint32_t positionY;

	uint32_t width;
	uint32_t height;
	HeroState* state;
	Environment* environment;
};

#endif /* SPRITE_H_ */
