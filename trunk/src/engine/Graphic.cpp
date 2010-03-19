/*
 * Graphic.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Graphic.h"
#include "Environment.h"

Graphic::Graphic(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame) {
	this->height = height;
	this->width = width;
	this->handles = handles;
	this->numberOfFrame = numberOfFrame;
	this->environment = 0;
}

Graphic::~Graphic() {
	/* Free memory */
	for(uint32_t i=0; i<numberOfFrame; i++) {
		delete[] handles[i];
	}
	delete[] handles;
}

void Graphic::update() {

}

void Graphic::render(VideoMemory* videoMemory) {
	VisibleArea* visibleArea = environment->getVisibleArea();

	uint32_t renderPositionX;
	uint32_t renderPositionY;

	uint32_t renderMaskX1 = 0;
	uint32_t renderMaskY1 = 0;
	uint32_t renderMaskX2 = 0;
	uint32_t renderMaskY2 = 0;

	//if(positionX < )

	//visibleArea
}

void Graphic::setPosition(uint32_t x, uint32_t y) {
	positionX = x;
	positionY = y;
}

void Graphic::set(Environment* environment) {
	this->environment = environment;
}
