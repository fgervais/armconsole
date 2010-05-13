/*
 * EnemyWalkingLeft.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "EnemyWalkingLeft.h"
#include "Bitmap.h"

EnemyWalkingLeft::EnemyWalkingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: EnemyState(width, height, frames, numberOfFrame) {

}

EnemyWalkingLeft::~EnemyWalkingLeft() {

}
