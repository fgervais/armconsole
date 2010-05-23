/*
 * MegamanOnGroundRight.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANRUNNINGRIGHT_H_
#define MEGAMANRUNNINGRIGHT_H_

#include "MegamanState.h"

class MegamanRunningRight: public MegamanState {
public:
	static MegamanState* getInstance();

	// Base class function override
	virtual void jump(Megaman*);
	virtual void runLeft(Megaman*);
	virtual void stopRunning(Megaman*);
	virtual void initialize(Megaman*);
	virtual void update(Megaman*);

private:
	MegamanRunningRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanRunningRight();

	static MegamanState* instance;
};

#endif /* MEGAMANRUNNINGRIGHT_H_ */
