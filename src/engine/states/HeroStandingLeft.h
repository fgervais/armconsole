/*
 * HeroStandingLeft.h
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#ifndef HEROSTANDINGLEFT_H_
#define HEROSTANDINGLEFT_H_

#include "HeroState.h"

class HeroStandingLeft: public HeroState {
public:
	HeroStandingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroStandingLeft();
};

#endif /* HEROSTANDINGLEFT_H_ */
