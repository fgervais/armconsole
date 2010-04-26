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

Level1::Level1()
	: Environment(520, 272, 56, 32) {

}

Level1::~Level1() {

}

void Level1::build() {
	Debug::writeLine("Entering Level1 build function");

	//Bitmap** black_square = new Bitmap*[2];
	//black_square[0] = new Bitmap("0:black.bmp");
	//black_square[1] = new Bitmap("0:gray.bmp");

	Bitmap** snow = new Bitmap*[1];
	snow[0] = new Bitmap("0:snow.bmp");

	Tile* tile1 = new Tile(56, 32, snow, 1);

	// Send the tiles to the environment (Base class)
	add(tile1, 0, 7);
	add(tile1, 1, 7);
	add(tile1, 2, 7);

	// Create a new visible area for the entire screen
	VisibleArea* visibleArea = new VisibleArea();
	visibleArea->x1 = 0;
	visibleArea->y1 = 0;
	visibleArea->x2 = 479;
	visibleArea->y2 = 271;

	// Send it to the environment (Base class)
	set(visibleArea);
}
