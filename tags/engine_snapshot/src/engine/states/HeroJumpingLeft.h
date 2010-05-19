/*
 * HeroJumpingLeft.h
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#ifndef HEROJUMPINGLEFT_H_
#define HEROJUMPINGLEFT_H_

#include "HeroState.h"

class HeroJumpingLeft: public HeroState {
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
	HeroJumpingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroJumpingLeft();

	static HeroState* instance;
};

#endif /* HEROJUMPINGLEFT_H_ */
