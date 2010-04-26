/*
 * Tile.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Tile.h"
#include "Environment.h"
#include "VideoMemory.h"
#include "VisibleArea.h"
#include "Bitmap.h"
#include "Debug.h"

Tile::Tile(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame) {
	this->height = height;
	this->width = width;
	this->frames = frames;
	this->numberOfFrame = numberOfFrame;
	this->currentFrame = 0;
	this->environment = 0;

	// Ensure every bitmap are loaded into memory
	for(uint32_t frameNb=0; frameNb<numberOfFrame; frameNb++) {
		if(!(frames[frameNb]->isLoaded())) {
			frames[frameNb]->load();
		}
	}
}

Tile::~Tile() {

}

void Tile::update() {
	if(currentFrame < (numberOfFrame-1)) {
		currentFrame++;
	}
	else {
		currentFrame = 0;
	}
}

/**
 * This function is used to render a graphic on the screen.
 *
 * For now this function doen't check if we are trying to
 * render something completely outside the screen. The
 * parent object should take care of that.
 *
 * @param videoMemory The video memory
 */
void Tile::render(VideoMemory* videoMemory) {
	if(environment == 0) {
		Debug::writeLine("I don't know the environment I belong to");
		return;
	}
	VisibleArea* visibleArea = environment->getVisibleArea();

	uint32_t renderPositionX = positionX - visibleArea->x1;
	uint32_t renderPositionY = positionY - visibleArea->y1;

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
	 * the visible area. If so, t set the render mask so it won't
	 * render the part outside the screen.
	 */
	if(positionX < visibleArea->x1) {
		renderMaskX1 = visibleArea->x1 - positionX;
		renderPositionX = visibleArea->x1;
	}
	if(positionY < visibleArea->y1) {
		renderMaskY1 = visibleArea->y1 - positionY;
		renderPositionY = visibleArea->y1;
	}
	if((positionX+width) > visibleArea->x2) {
		renderMaskX2 = visibleArea->x2 - positionX;
	}
	if((positionY+height) > visibleArea->y2) {
		renderMaskY2 = visibleArea->y2 - positionY;
	}

	// Draw the image on the screen
	uint32_t videoMemoryWidth = videoMemory->getWidth();
	uint32_t* videoMemoryPointer = videoMemory->getPointer();

	for (uint32_t i=renderMaskY1; i<renderMaskY2; i++) {
		for (uint32_t j=renderMaskX1; j<renderMaskX2; j++) {
			videoMemoryPointer[(i+renderPositionY)*videoMemoryWidth + (j+renderPositionX)]
			                   = frames[currentFrame]->getData()[i*width + j];
		}
	}

}

/**
 * Set the absolute position in the environment.
 *
 * @param x X position
 * @param y Y position
 */
void Tile::setPosition(uint32_t x, uint32_t y) {
	positionX = x;
	positionY = y;
}

void Tile::set(Environment* environment) {
	this->environment = environment;
}
