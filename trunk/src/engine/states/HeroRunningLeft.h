/*
 * HeroOnGroundLeft.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HERORUNNINGLEFT_H_
#define HERORUNNINGLEFT_H_

#include "HeroState.h"

class HeroRunningLeft: public HeroState {
public:
	static HeroState* getInstance();

	// Base class function override
	virtual void jump(Hero*);
	virtual void runRight(Hero*);
	virtual void stopRunning(Hero*);
	virtual void initialize(Hero*);
	virtual void update(Hero*);

private:
	HeroRunningLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroRunningLeft();

	static HeroState* instance;
};

#endif /* HERORUNNINGLEFT_H_ */
