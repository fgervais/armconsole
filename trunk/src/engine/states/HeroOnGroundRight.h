/*
 * HeroOnGroundRight.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HEROONGROUNDRIGHT_H_
#define HEROONGROUNDRIGHT_H_

#include "HeroState.h"

class HeroOnGroundRight: public HeroState {
public:
	HeroOnGroundRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroOnGroundRight();
};

#endif /* HEROONGROUNDRIGHT_H_ */
