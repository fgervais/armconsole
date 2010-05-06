/*
 * HeroState.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HEROSTATE_H_
#define HEROSTATE_H_

#include "Synchronized.h"

#include <stdint.h>

class Sprite;
class Bitmap;
class VideoMemory;

class HeroState {
public:
	HeroState(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroState();

	/*
	 * Since this function had to be static,
	 * it cannot be an abstract function of
	 * the base class.
	 */
	// Abstract singleton function
	//virtual HeroState* getInstance() = 0;

	// Finite State Machine functions
	// All these do nothing by default
	virtual void jump(Sprite*) {};
	virtual void runLeft(Sprite*) {};
	virtual void runRight(Sprite*) {};
	virtual void stop(Sprite*) {};
	virtual void initialize(Sprite*) {};

	// Accessors
	uint32_t getWidth() { return width; }
	uint32_t getHeight() { return height; }
	void setLoopFirstFrame(uint32_t frameNumber);

	Bitmap* getCurrentFrame() { return frames[currentFrame]; }

	virtual void reset();
	virtual void update(Sprite*);
	virtual void render(Sprite*, VideoMemory*);

private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
	uint32_t numberOfFrame;
	uint32_t currentFrame;
	uint32_t loopFirstFrame;
	uint32_t positionX;
	uint32_t positionY;
};

#endif /* HEROSTATE_H_ */
