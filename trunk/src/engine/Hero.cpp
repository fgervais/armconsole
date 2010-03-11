/*
 * Hero.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Hero.h"

Hero::Hero(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame)
	: Sprite(height, width, handles, numberOfFrame) {

	set(Right);
}

Hero::~Hero() {

}
