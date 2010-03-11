/*
 * Hero.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef HERO_H_
#define HERO_H_

#include "Sprite.h"

class Hero: public Sprite {
public:
	Hero(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame);
	virtual ~Hero();
};

#endif /* HERO_H_ */
