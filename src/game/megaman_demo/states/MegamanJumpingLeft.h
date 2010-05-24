/*
 * MegamanJumpingLeft.h
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANJUMPINGLEFT_H_
#define MEGAMANJUMPINGLEFT_H_

#include "MegamanState.h"

class Wave;

class MegamanJumpingLeft: public MegamanState {
public:
	static MegamanState* getInstance();

	// Base class function override
	virtual void runLeft(Megaman*);
	virtual void runRight(Megaman*);
	virtual void stopRunning(Megaman*);
	virtual void stopJumping(Megaman*);
	virtual void initialize(Megaman*);
	virtual void update(Megaman*);

private:
	MegamanJumpingLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanJumpingLeft();

	static MegamanState* instance;
};

#endif /* MEGAMANJUMPINGLEFT_H_ */
