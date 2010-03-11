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
#include "Perspective.h"

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
}

Environment::~Environment() {

}

void Environment::render(VideoMemory* videoMemory) {
	Hero::Direction heroDirection = hero->getDirection();

	if(heroDirection == Hero::Up) {
		scrollUp(background, hero->getVelocityY());
	}
	else if(heroDirection == Hero::Down) {
		scrollDown(background, -(hero->getVelocityY()));
	}
	else if(heroDirection == Hero::Left) {
		scrollLeft(background, hero->getVelocityX());
	}
	else if(heroDirection == Hero::Right) {
		scrollRight(background, -(hero->getVelocityX()));
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
 * @return 0 if the hero is successfully set. 1 otherwise.
 */
uint8_t Environment::set(Hero* hero, uint32_t x, uint32_t y) {
	this->hero = hero;
	hero->setPosition(x,y);
	return 0;
}

/**
 * Set the environment background.
 *
 * @param background Background to be set
 * @return 0 if the background is successfully set. 1 otherwise.
 */
uint8_t Environment::set(Background* background) {
	this->background = background;
	return 0;
}

/**
 * Set the environment physics.
 *
 * @param physics Physics to be set
 * @return 0 if the physics is successfully set. 1 otherwise.
 */
uint8_t Environment::set(Physics* physics) {
	this->physics = physics;
	return 0;
}

/*
 * PRIVATE FUNCTIONS
 */

void Environment::scrollUp(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollDown(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollLeft(Background* background, uint32_t numberOfPixel) {

}

void Environment::scrollRight(Background* background, uint32_t numberOfPixel) {

}
