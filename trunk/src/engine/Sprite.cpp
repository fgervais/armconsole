/*
 * Sprite.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Sprite.h"
#include "Environment.h"

Sprite::Sprite(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame)
	: Graphic(height, width, handles, numberOfFrame) {


}

Sprite::~Sprite() {

}
