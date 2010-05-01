/*
 * HeroInAirLeft.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HEROINAIRLEFT_H_
#define HEROINAIRLEFT_H_

#include "HeroState.h"

class HeroInAirLeft: public HeroState {
public:
	HeroInAirLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroInAirLeft();
};

#endif /* HEROINAIRLEFT_H_ */
