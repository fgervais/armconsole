/*
 * Background.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#include "Background.h"

Background::Background(char* path, uint32_t height, uint32_t width) {
	this->path = path;
	this->height = height;
	this->width = width;
}

Background::~Background() {

}
