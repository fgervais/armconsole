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

Environment::Environment(uint32_t height, uint32_t width, uint32_t tileHeight, uint32_t tileWidth) {
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
}

Environment::~Environment() {

}

void Environment::render(VideoMemory* videoMemory) {
	Hero::Direction heroDirection = hero->getDirection();
	int32_t xHeroVelocity = hero->getVelocityX();

	visibleArea->x1 += xHeroVelocity;
	visibleArea->x2 += xHeroVelocity;

	if(heroDirection == Hero::Left) {
		// Render background

		// Add new background section

		// Render tiles
		for(uint32_t i=visibleArea->y1/tileHeight; i<=visibleArea->y2/tileHeight; i++) {
			for(uint32_t j=visibleArea->x1/tileWidth; j<(visibleArea->x2/tileWidth); j++) {
				tileMap[i][j]->render(videoMemory);
			}
		}
	}
	else if(heroDirection == Hero::Right) {
		scrollLeft(background, -(xHeroVelocity));
	}

}

void Environment::update() {

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
		tile->setPosition(x*tileWidth, y*tileHeight);
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

void Environment::renderBackground() {

}

void Environment::scrollUp(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollDown(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollLeft(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollRight(Background* background, uint32_t numberOfPixel) {

}
