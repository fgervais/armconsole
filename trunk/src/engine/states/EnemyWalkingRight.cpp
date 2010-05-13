/*
 * EnemyWalkingRight.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "EnemyWalkingRight.h"
#include "Bitmap.h"

EnemyWalkingRight::EnemyWalkingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame)
: EnemyState(width, height, frames, numberOfFrame) {

}

EnemyWalkingRight::~EnemyWalkingRight() {

}
