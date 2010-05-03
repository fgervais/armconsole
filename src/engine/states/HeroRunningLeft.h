/*
 * HeroOnGroundLeft.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HERORUNNINGLEFT_H_
#define HERORUNNINGLEFT_H_

#include "HeroState.h"

class HeroRunningLeft: public HeroState {
public:
	HeroRunningLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroRunningLeft();
};

#endif /* HERORUNNINGLEFT_H_ */
