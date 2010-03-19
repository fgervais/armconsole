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

Tile::Tile(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame)
	: Graphic(height, width, handles, numberOfFrame) {


}

Tile::~Tile() {

}
