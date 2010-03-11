/*
 * Background.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#include "Background.h"

Background::Background(uint32_t* handle, uint32_t height, uint32_t width) {
	this->handle = handle;
	this->height = height;
	this->width = width;
}

Background::~Background() {

}
