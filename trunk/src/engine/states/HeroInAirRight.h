/*
 * HeroInAirRight.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HEROINAIRRIGHT_H_
#define HEROINAIRRIGHT_H_

#include "HeroState.h"

class HeroInAirRight: public HeroState {
public:
	HeroInAirRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroInAirRight();
};

#endif /* HEROINAIRRIGHT_H_ */
