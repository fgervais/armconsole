/*
 * HeroState.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANSTATE_H_
#define MEGAMANSTATE_H_

#include "State.h"

#include <stdint.h>

class Megaman;

class MegamanState : public State {
public:
	MegamanState(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanState();

	/*
	 * Since this function had to be static,
	 * it cannot be an abstract function of
	 * the base class.
	 */
	// Abstract singleton function
	//virtual HeroState* getInstance() = 0;

	virtual void update(Megaman*) {}
	virtual void initialize(Megaman*) {}

	// Finite State Machine functions
	// All these do nothing by default
	virtual void jump(Megaman*) {}
	virtual void runLeft(Megaman*) {}
	virtual void runRight(Megaman*) {}
	virtual void stopRunning(Megaman*) {}
	virtual void stopJumping(Megaman*) {}
};

#endif /* MEGAMANSTATE_H_ */
