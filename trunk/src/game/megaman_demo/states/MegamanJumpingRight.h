/*
 * HeroJumpingRight.h
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANJUMPINGRIGHT_H_
#define MEGAMANJUMPINGRIGHT_H_

#include "MegamanState.h"

class Megaman;

class MegamanJumpingRight: public MegamanState {
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
	MegamanJumpingRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanJumpingRight();

	static MegamanState* instance;
};

#endif /* MEGAMANJUMPINGRIGHT_H_ */
