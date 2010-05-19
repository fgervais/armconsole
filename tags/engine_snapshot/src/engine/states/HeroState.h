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
class Hero;

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
	virtual void jump(Hero*) {};
	virtual void runLeft(Hero*) {};
	virtual void runRight(Hero*) {};
	virtual void stopRunning(Hero*) {};
	virtual void stopJumping(Hero*) {};
	virtual void initialize(Hero*) {};

	// Accessors
	uint32_t getWidth();
	uint32_t getHeight();

	Bitmap* getCurrentFrame() { return frames[currentFrame]; }

	virtual void reset();
	virtual void update(Hero*) {};
	virtual void render(Hero*, VideoMemory*);

protected:
	// Subclass may need to update the current frame manually
	uint32_t currentFrame;
	uint32_t numberOfFrame;

private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
};

#endif /* HEROSTATE_H_ */
