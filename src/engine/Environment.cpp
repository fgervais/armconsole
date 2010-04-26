/*
 * Environment.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Environment.h"
#include "Sprite.h"
#include "Tile.h"
#include "Hero.h"
#include "Background.h"
#include "Physics.h"
#include "VisibleArea.h"
#include "Debug.h"
#include "VideoMemory.h"

Environment::Environment(uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight) {
	this->height = height;
	this->width = width;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;

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

	// Sprite limit without the Hero
	// TODO: Should the spriteLimit be stored somewhere else?
	spriteLimit = 5;
	numberOfSprite = 0;

	sprites = new Sprite*[spriteLimit];
	hero = 0;
	background = 0;
	physics = 0;
	visibleArea = 0;
	//Debug::writeLine("Environment constructor done");
}

Environment::~Environment() {

}

void Environment::render(VideoMemory* videoMemory) {
	// Render background
	renderBackground(videoMemory);

	// Render tiles
	renderTiles(videoMemory);

	// Render Hero
	renderHero(videoMemory);

	// Render sprites
	renderSprites(videoMemory);

}

void Environment::update() {
	//int32_t heroVelocityX = hero->getVelocityX();

	//visibleArea->x1 += heroVelocityX;
	//visibleArea->x2 += heroVelocityX;

	/*if(visibleArea->x2 < (width-1)) {
		visibleArea->x1 += 5;
		visibleArea->x2 += 5;
	}
	else {
		visibleArea->x1 = 0;
		visibleArea->x2 = 479;
	}*/

	// Update background
	updateBackground();

	// Update tiles
	updateTiles();

	// Update hero
	updateHero();

	// Update sprites
	updateSprites();
}

/*
 * PROTECTED FUNCTIONS
 */

/**
 * Add a sprite in the environment.
 *
 * Note that there's a sprite limit.
 *
 * @param sprite Spite to be added
 * @param x X position in pixel
 * @param y Y position in pixel
 * @return 0 if the sprite is successfully added. 1 otherwise.
 */
uint8_t Environment::add(Sprite* sprite, uint32_t x, uint32_t y) {
	if(numberOfSprite < spriteLimit) {
		sprites[numberOfSprite] = sprite;
		// Set the pixel position of the sprite
		sprite->setPosition(x,y);
		numberOfSprite++;
		return 0;
	}
	return 1;
}

/**
 * Add a tile to the tile map.
 *
 * @param tile Tile to be added
 * @param x X position of the tile in the tile map
 * @param y Y position of the tile in the tile map
 * @return 0 if the tile is successfully added. 1 otherwise.
 */
uint8_t Environment::add(Tile* tile, uint32_t x, uint32_t y) {
	if(x < widthInTile && y < heightInTile) {
		tileMap[y][x] = tile;
		// The tile needs to know its parent environment
		tile->set(this);
		//tile->setPosition(x*tileWidth, y*tileHeight);
		return 0;
	}
	return 1;
}

/**
 * Set the Hero.
 *
 * @param hero Hero to be set.
 * @param x X position in pixel
 * @param y Y position in pixel
 */
void Environment::set(Hero* hero, uint32_t x, uint32_t y) {
	this->hero = hero;
	hero->setPosition(x,y);
}

/**
 * Set the environment background.
 *
 * @param background Background to be set
 */
void Environment::set(Background* background) {
	this->background = background;
}

/**
 * Set the environment physics.
 *
 * @param physics Physics to be set
 */
void Environment::set(Physics* physics) {
	this->physics = physics;
}

void Environment::set(VisibleArea* visibleArea) {
	this->visibleArea = visibleArea;
}

/*
 * PRIVATE FUNCTIONS
 */

void Environment::renderBackground(VideoMemory* videoMemory) {
	// Set white background for now
	uint32_t* lcd_ptr = videoMemory->getPointer();
	uint32_t bufferLength = videoMemory->getWidth()*videoMemory->getHeight();

	for (uint32_t i=0; i<bufferLength; i++) {
		*(lcd_ptr++) = 0x00FF6633;
	}
}

void Environment::renderTiles(VideoMemory* videoMemory) {
	for(uint32_t i=visibleArea->y1/tileHeight; i<visibleArea->y2/tileHeight; i++) {
		for(uint32_t j=visibleArea->x1/tileWidth; j<(visibleArea->x2/tileWidth); j++) {
			// Set the tile position - This is subject to change
			if(tileMap[i][j] != 0) {
				/*if(i == 10 && j == 10) {
					Debug::writeLine("Rendering tile [10,10]");
				}
				else {
					Debug::writeLine("Rendering other tile");
				}*/
				tileMap[i][j]->setPosition(j*tileWidth, i*tileHeight);
				tileMap[i][j]->render(videoMemory);
			}
		}
	}
}

void Environment::renderHero(VideoMemory* videoMemory) {

}

void Environment::renderSprites(VideoMemory* videoMemory) {

}

void Environment::updateBackground() {

}

void Environment::updateTiles() {
	for(uint32_t i=visibleArea->y1/tileHeight; i<visibleArea->y2/tileHeight; i++) {
		for(uint32_t j=visibleArea->x1/tileWidth; j<(visibleArea->x2/tileWidth); j++) {
			if(tileMap[i][j] != 0) {
				tileMap[i][j]->update();
			}
		}
	}
}

void Environment::updateHero() {

}

void Environment::updateSprites() {

}
