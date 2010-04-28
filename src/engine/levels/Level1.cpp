/*
 * Level1.cpp
 *
 *  Created on: Mar 11, 2010
 *      Author: fgervais
 */

#include "Level1.h"
#include "Tile.h"
#include "Bitmap.h"
#include "VisibleArea.h"
#include "Debug.h"
#include "Background.h"

Level1::Level1()
	: Environment(960, 272, 56, 32) {

}

Level1::~Level1() {

}

void Level1::build() {
	Debug::writeLine("Entering Level1 build function");

	//Bitmap** black_square = new Bitmap*[2];
	//black_square[0] = new Bitmap("0:black.bmp");
	//black_square[1] = new Bitmap("0:gray.bmp");

	Bitmap** snow = new Bitmap*[1];
	snow[0] = new Bitmap("0:snow1.bmp");

	Tile* tile1 = new Tile(56, 32, snow, 1, this);

	Bitmap** brick = new Bitmap*[1];
	brick[0] = new Bitmap("0:brick.bmp");

	Tile* tile2 = new Tile(56, 32, brick, 1, this);

	// Send the tiles to the environment (Base class)
	for(uint8_t i=0; i<18; i++) {
		add(tile1, i, 7);
	}

	for(uint8_t i=0; i<18; i++) {
		add(tile2, i, 8);
	}

	// Create a new visible area for the entire screen
	VisibleArea* visibleArea = new VisibleArea();
	visibleArea->x1 = 0;
	visibleArea->y1 = 0;
	visibleArea->x2 = 480;
	visibleArea->y2 = 272;

	// Send it to the environment (Base class)
	set(visibleArea);

	Bitmap* background_bitmap = new Bitmap("0:back4.bmp");
	Background* background = new Background(background_bitmap, 768, 272, this);
	//background->setScrollSpeedMultipliers(0.5, 0.5);

	set(background);
}
