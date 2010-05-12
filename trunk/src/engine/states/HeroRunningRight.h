/*
 * HeroOnGroundRight.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HERORUNNINGRIGHT_H_
#define HERORUNNINGRIGHT_H_

#include "HeroState.h"

class HeroRunningRight: public HeroState {
public:
	static HeroState* getInstance();

	// Base class function override
	virtual void jump(Hero*);
	virtual void runLeft(Hero*);
	virtual void stopRunning(Hero*);
	virtual void initialize(Hero*);
	virtual void update(Hero*);

private:
	HeroRunningRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroRunningRight();

	static HeroState* instance;
};

#endif /* HERORUNNINGRIGHT_H_ */
