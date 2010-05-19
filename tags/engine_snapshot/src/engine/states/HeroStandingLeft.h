/*
 * HeroStandingLeft.h
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#ifndef HEROSTANDINGLEFT_H_
#define HEROSTANDINGLEFT_H_

#include "HeroState.h"

class HeroStandingLeft: public HeroState {
public:
	static HeroState* getInstance();

	// Base class function override
	virtual void jump(Hero*);
	virtual void runLeft(Hero*);
	virtual void runRight(Hero*);
	virtual void update(Hero*);
	virtual void initialize(Hero*);

private:
	HeroStandingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroStandingLeft();

	static HeroState* instance;
};

#endif /* HEROSTANDINGLEFT_H_ */
