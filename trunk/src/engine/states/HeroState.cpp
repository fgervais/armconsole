/*
 * HeroState.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "HeroState.h"
#include "Bitmap.h"

HeroState::HeroState(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame) {
	this->width = width;
	this->height = height;
	this->frames = frames;
	this->numberOfFrame = numberOfFrame;
	this->currentFrame = 0;
	this->loopFirstFrame = 0;

	// Ensure every bitmap are loaded into memory
	for(uint32_t frameNumber=0; frameNumber<numberOfFrame; frameNumber++) {
		if(!(frames[frameNumber]->isLoaded())) {
			frames[frameNumber]->load();
		}
	}

}

HeroState::~HeroState() {

}

/**
 * Set the first frame of the frame loop.
 *
 * Sometime the first frame should be displayed only once when
 * entering the state. In that case, the first frame of the
 * loop should be set to the second frame i.e. frame1.
 *
 * The class default to frame0 for the loop first frame.
 *
 * @param frameNumber Loop first frame number.
 */
void HeroState::setLoopFirstFrame(uint32_t frameNumber) {
	loopFirstFrame = frameNumber;
}

/**
 * Reset the state.
 *
 * Currently only reset the frame counter to 0.
 */
void HeroState::reset() {
	currentFrame = 0;
}

/**
 * Update the current state.
 *
 * This function should be called on every engine tick. Even if this
 * function has to be overridden by some subclass, it should be called
 * anyway by the subclass as it update the frame pointer. It doesn't
 * matter if you call it at the beginning or at the end of your functions.
 *
 * @param sprite Sprite which should be updated by the state.
 */
void HeroState::update(Sprite* sprite) {
	if(currentFrame < (numberOfFrame-1)) {
		currentFrame++;
	}
	else {
		currentFrame = loopFirstFrame;
	}
}
