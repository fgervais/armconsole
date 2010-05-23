/*
 * MetoolState.cpp
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#include "MetoolState.h"

MetoolState::MetoolState(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks)
	: State(animationWidth, animationHeight, animationFrames, numberOfFrame, animationMasks) {
}

MetoolState::~MetoolState() {

}
