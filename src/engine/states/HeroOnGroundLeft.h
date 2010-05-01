/*
 * HeroOnGroundLeft.h
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#ifndef HEROONGROUNDLEFT_H_
#define HEROONGROUNDLEFT_H_

#include "HeroState.h"

class HeroOnGroundLeft: public HeroState {
public:
	HeroOnGroundLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroOnGroundLeft();
};

#endif /* HEROONGROUNDLEFT_H_ */
