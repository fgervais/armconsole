/*
 * MegamanState.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "MegamanState.h"

MegamanState::MegamanState(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
	: State(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {

}

MegamanState::~MegamanState() {

}
