/*
 * HeroJumpingRight.h
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#ifndef HEROJUMPINGRIGHT_H_
#define HEROJUMPINGRIGHT_H_

#include "HeroState.h"

class Sprite;

class HeroJumpingRight: public HeroState {
public:
	static HeroState* getInstance();

	// Base class function override
	virtual void runLeft(Hero*);
	virtual void runRight(Hero*);
	virtual void stopRunning(Hero*);
	virtual void stopJumping(Hero*);
	virtual void initialize(Hero*);
	virtual void update(Hero*);

private:
	HeroJumpingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroJumpingRight();

	static HeroState* instance;
};

#endif /* HEROJUMPINGRIGHT_H_ */
