/*
 * Graphic.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Graphic.h"

Graphic::Graphic(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame) {
	this->height = height;
	this->width = width;
	this->handles = handles;
	this->numberOfFrame = numberOfFrame;
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

void Graphic::render(VideoMemory*) {

}

void Graphic::setPosition(uint32_t x, uint32_t y) {
	xPosition = x;
	yPosition = y;
}
