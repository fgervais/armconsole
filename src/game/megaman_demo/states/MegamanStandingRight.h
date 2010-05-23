/*
 * MegamanStandingRight.h
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANSTANDINGRIGHT_H_
#define MEGAMANSTANDINGRIGHT_H_

#include "MegamanState.h"

class MegamanStandingRight : public MegamanState {
public:
	static MegamanState* getInstance();

	// Base class function override
	virtual void jump(Megaman*);
	virtual void runLeft(Megaman*);
	virtual void runRight(Megaman*);
	virtual void update(Megaman*);
	virtual void initialize(Megaman*);

private:
	MegamanStandingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MegamanStandingRight();

	static MegamanState* instance;
};

#endif /* MEGAMANSTANDINGRIGHT_H_ */
