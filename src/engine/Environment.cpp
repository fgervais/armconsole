/*
 * Environment.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Environment.h"

Environment::Environment(uint32_t height, uint32_t width, uint32_t tileHeight, uint32_t tileWidth) {
	this->height = height;
	this->width = width;

	heightInTile = height/tileHeight;
	widthInTile = width/tileWidth;

	tileMap = new Tile**[heightInTile];
	for(uint32_t i=0; i<widthInTile; i++) {
		tileMap[i] = new Tile*[widthInTile];
	}

	// Initialize the tile array
	for(uint32_t i=0; i<heightInTile; i++) {
		for(uint32_t j=0; j<widthInTile; j++) {
			tileMap[i][j] = 0;
		}
	}
}

Environment::~Environment() {

}
