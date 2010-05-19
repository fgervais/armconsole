/*
 * Hero.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef HERO_H_
#define HERO_H_

#include "Sprite.h"

#include <stdint.h>

class Environment;
class HeroState;
class VideoMemory;

class Hero : public Sprite {
public:
	Hero(HeroState* initialState, Environment* environment);
	virtual ~Hero();

	// Accessors
	virtual uint32_t getWidth();
	virtual uint32_t getHeight();

	void setState(HeroState* state);

	virtual void update();
	virtual void render(VideoMemory*);

	// Action functions
	void jump();
	void runLeft();
	void runRight();
	void stopRunning();
	void stopJumping();
private:
	HeroState* state;
};

#endif /* HERO_H_ */
