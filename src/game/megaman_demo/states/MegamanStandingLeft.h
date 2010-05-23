/*
 * MegamanStandingLeft.h
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANSTANDINGLEFT_H_
#define MEGAMANSTANDINGLEFT_H_

#include "MegamanState.h"

class MegamanStandingLeft: public MegamanState {
public:
	static MegamanState* getInstance();

	// Base class function override
	virtual void jump(Megaman*);
	virtual void runLeft(Megaman*);
	virtual void runRight(Megaman*);
	virtual void update(Megaman*);
	virtual void initialize(Megaman*);

private:
	MegamanStandingLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanStandingLeft();

	static MegamanState* instance;
};

#endif /* MEGAMANSTANDINGLEFT_H_ */
