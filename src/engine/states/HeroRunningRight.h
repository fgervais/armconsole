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
	HeroRunningRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroRunningRight();

	virtual HeroState* getInstance();

private:
	HeroState* instance;
};

#endif /* HERORUNNINGRIGHT_H_ */
