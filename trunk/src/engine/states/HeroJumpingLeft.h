/*
 * HeroJumpingLeft.h
 *
 *  Created on: May 7, 2010
 *      Author: fgervais
 */

#ifndef HEROJUMPINGLEFT_H_
#define HEROJUMPINGLEFT_H_

#include "HeroState.h"

class HeroJumpingLeft: public HeroState {
public:
	HeroJumpingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroJumpingLeft();
};

#endif /* HEROJUMPINGLEFT_H_ */
