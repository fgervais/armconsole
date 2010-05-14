/*
 * HeroState.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: fgervais
 */

#include "HeroState.h"
#include "Bitmap.h"
#include "VideoMemory.h"
#include "Hero.h"
#include "Environment.h"
#include "VisibleArea.h"
#include "Debug.h"

HeroState::HeroState(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame) {
	this->width = width;
	this->height = height;
	this->frames = frames;
	this->numberOfFrame = numberOfFrame;
	this->currentFrame = 0;

	// Ensure every bitmap are loaded into memory
	for(uint32_t frameNumber=0; frameNumber<numberOfFrame; frameNumber++) {
		if(!(frames[frameNumber]->isLoaded())) {
			frames[frameNumber]->load();
		}
	}

}

HeroState::~HeroState() {

}

uint32_t HeroState::getWidth() {
	return width;
}

uint32_t HeroState::getHeight() {
	return height;
}

/**
 * Reset the state.
 *
 * Currently only reset the frame counter to 0.
 */
void HeroState::reset() {
	currentFrame = 0;
}

void HeroState::render(Hero* sprite, VideoMemory* videoMemory) {
	VisibleArea* visibleArea = sprite->getEnvironment()->getVisibleArea();
	uint32_t positionX = sprite->getPositionX();
	uint32_t positionY = sprite->getPositionY();

	// Get x,y coordinates inside the visible area
	uint32_t renderPositionX = positionX - visibleArea->x;
	uint32_t renderPositionY = positionY - visibleArea->y;

	/*
	 * These are used to subtract part of the image from the
	 * rendering process.
	 */
	uint32_t renderMaskX1 = 0;
	uint32_t renderMaskY1 = 0;
	uint32_t renderMaskX2 = width;
	uint32_t renderMaskY2 = height;

	/*
	 * These 4 following tests check if part of the image is outside
	 * the visible area. If so, set the render mask so it won't
	 * render the part outside the screen.
	 */
	if(positionX < visibleArea->x) {
		renderMaskX1 = visibleArea->x - positionX;
		renderPositionX = 0;
	}
	if(positionY < visibleArea->y) {
		renderMaskY1 = visibleArea->y - positionY;
		renderPositionY = 0;
	}
	if((positionX+width) > (visibleArea->x+visibleArea->width)) {
		renderMaskX2 = (visibleArea->x+visibleArea->width) - positionX;
	}
	if((positionY+height) > (visibleArea->y+visibleArea->height)) {
		renderMaskY2 = (visibleArea->y+visibleArea->height) - positionY;
	}

	// Draw the image on the screen
	uint32_t videoMemoryWidth = videoMemory->getWidth();
	uint32_t* videoMemoryPointer = videoMemory->getPointer();

	// Render the part of the tile inside the render mask
	for (uint32_t i=renderMaskY1; i<renderMaskY2; i++) {
		for (uint32_t j=renderMaskX1; j<renderMaskX2; j++) {
			// This is complicated to understand but I don't think we can simplify it
			videoMemoryPointer[((i-renderMaskY1)+renderPositionY)*videoMemoryWidth + ((j-renderMaskX1)+renderPositionX)]
							   = frames[currentFrame]->getData()[i*width + j];
		}
	}
}
