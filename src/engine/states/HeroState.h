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

class HeroState {
public:
	HeroState(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroState();

	// Abstract singleton function
	//virtual HeroState* getInstance() = 0;

	// Finite State Machine functions
	// All these does nothing by default
	virtual void jump(Sprite*) {};
	virtual void runLeft(Sprite*) {};
	virtual void runRight(Sprite*) {};
	virtual void stall(Sprite*) {};

	// Accessors
	uint32_t getWidth() { return width; }
	uint32_t getHeight() { return height; }
	void setLoopFirstFrame(uint32_t frameNumber);

	Bitmap* getCurrentFrame() { return frames[currentFrame]; }
	void reset();
	virtual void update(Sprite*);

private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
	uint32_t numberOfFrame;
	uint32_t currentFrame;
	uint32_t loopFirstFrame;
};

#endif /* HEROSTATE_H_ */
