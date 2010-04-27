/*
 * Background.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#include "Background.h"
#include "Environment.h"
#include "VideoMemory.h"
#include "VisibleArea.h"
#include "Bitmap.h"

Background::Background(Bitmap* bitmap, uint32_t width, uint32_t height, Environment* environment) {
	this->bitmap = bitmap;
	this->height = height;
	this->width = width;

	this->environment = environment;

	xSpeedMultiplier = 1.0;
	ySpeedMultiplier = 1.0;

	if(!bitmap->isLoaded()) {
		bitmap->load();
	}
}

Background::~Background() {

}

void Background::render(VideoMemory* videoMemory) {
	VisibleArea* visibleArea = environment->getVisibleArea();

	/*
	 * Calculate the visible area of the background according to the speed multiplier.
	 *
	 * We do this because the background normally scroll slower than the layer
	 * where the hero is.
	 */
	uint32_t renderX1 = (uint32_t)(((float)visibleArea->x1) * xSpeedMultiplier);
	uint32_t renderY1 = (uint32_t)(((float)visibleArea->y1) * ySpeedMultiplier);

	uint32_t renderWidth = visibleArea->x2 - visibleArea->x1;
	uint32_t renderHeight = visibleArea->y2 - visibleArea->y1;

	// These are safe initial guess
	uint32_t renderMaskX1 = renderX1 % width;
	uint32_t renderMaskY1 = renderY1 % height;

	// Draw the image on the screen
	uint32_t videoMemoryWidth = videoMemory->getWidth();
	uint32_t* videoMemoryPointer = videoMemory->getPointer();

	/* Generic version */
	/*for (uint32_t i=0; i<renderHeight; i++) {
		for (uint32_t j=0; j<renderWidth; j++) {
			videoMemoryPointer[i*videoMemoryWidth + j]
			                   = bitmap->getData()[((i+renderMaskY1) % height)*width + ((j+renderMaskX1) % width)];
		}
	}*/

	/* Optimized version */
	uint32_t* image = bitmap->getData();
	for (uint32_t i=0; i<renderHeight; i++) {
		for (uint32_t j=0; j<renderWidth; j++) {
			videoMemoryPointer[i*videoMemoryWidth + j]
							   = image[(i+renderMaskY1)*width + ((j+renderMaskX1) & (width-1))];
		}
	}

}

void Background::set(Environment* environment) {
	this->environment = environment;
}

void Background::setScrollSpeedMultipliers(float x, float y) {
	this->xSpeedMultiplier = x;
	this->ySpeedMultiplier = y;
}

/* PRIVATE FUNCTIONS */


