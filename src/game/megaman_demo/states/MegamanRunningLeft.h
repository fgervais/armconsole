/*
 * MegamanOnGroundLeft.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANRUNNINGLEFT_H_
#define MEGAMANRUNNINGLEFT_H_

#include "MegamanState.h"

class MegamanRunningLeft: public MegamanState {
public:
	static MegamanState* getInstance();

	// Base class function override
	virtual void jump(Megaman*);
	virtual void runRight(Megaman*);
	virtual void stopRunning(Megaman*);
	virtual void initialize(Megaman*);
	virtual void update(Megaman*);

private:
	MegamanRunningLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanRunningLeft();

	static MegamanState* instance;
};

#endif /* MEGAMANRUNNINGLEFT_H_ */
