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

	Bitmap** brick = new Bitmap*[1];
	brick[0] = new Bitmap("0:brick.bmp");

	Tile* tile2 = new Tile(56, 32, brick, 1);

	// Send the tiles to the environment (Base class)
	add(tile1, 0, 7);
	add(tile1, 1, 7);
	add(tile1, 2, 7);
	add(tile1, 3, 7);
	add(tile1, 4, 7);
	add(tile1, 5, 7);
	add(tile1, 6, 7);
	add(tile1, 7, 7);
	add(tile1, 8, 7);

	add(tile2, 0, 8);
	add(tile2, 1, 8);
	add(tile2, 2, 8);
	add(tile2, 3, 8);
	add(tile2, 4, 8);
	add(tile2, 5, 8);
	add(tile2, 6, 8);
	add(tile2, 7, 8);
	add(tile2, 8, 8);

	// Create a new visible area for the entire screen
	VisibleArea* visibleArea = new VisibleArea();
	visibleArea->x1 = 0;
	visibleArea->y1 = 0;
	visibleArea->x2 = 480;
	visibleArea->y2 = 272;

	// Send it to the environment (Base class)
	set(visibleArea);
}
