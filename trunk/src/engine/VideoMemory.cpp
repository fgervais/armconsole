/*
 * VideoMemory.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#include "VideoMemory.h"

VideoMemory::VideoMemory(uint32_t* pointer, uint32_t width, uint32_t height) {
	this->pointer = pointer;
	this->height = height;
	this->width = width;
}

VideoMemory::~VideoMemory() {

}
