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
	virtual HeroState* getInstance() = 0;

	// Finite State Machine functions
	virtual void jump(Sprite*) {};
	virtual void runLeft(Sprite*) {};
	virtual void runRight(Sprite*) {};

	Bitmap* getCurrentFrame() { return frames[currentFrame]; }
	void reset();
	virtual void update(Sprite*);

private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
	uint32_t numberOfFrame;
	uint32_t currentFrame;
};

#endif /* HEROSTATE_H_ */
