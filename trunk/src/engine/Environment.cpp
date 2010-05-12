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
	if((height % tileHeight) > 0) {
		heightInTile++;
	}

	widthInTile = width/tileWidth;
	if((width % tileWidth) > 0) {
		widthInTile++;
	}

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
	/*static int8_t velocity = 10;

	if(visibleArea->x2+velocity > width) {
		velocity = -10;
	}
	else if((int32_t)visibleArea->x1+velocity < 0) {
		velocity = 10;
	}

	visibleArea->x1 += velocity;
	visibleArea->x2 += velocity;*/

	uint32_t heroMiddlePosition = hero->getPositionX() + (hero->getWidth() / 2);

	// TODO: Fix hard coded values
	// VisibleArea should contain Width and Height instead of x2, y2
	if((heroMiddlePosition - 240) < 0) {
		visibleArea->x1 = 0;
		visibleArea->x2 = 480;
	}
	else if((heroMiddlePosition + 240) > width) {
		visibleArea->x1 = width - 480;
		visibleArea->x2 = width;
	}
	else {
		visibleArea->x1 = heroMiddlePosition - 240;
		visibleArea->x2 = heroMiddlePosition + 240;
	}

	// Update background
	updateBackground();

	// Update tiles
	updateTiles();

	// Update hero
	updateHero();

	// Update sprites
	updateSprites();
}

/**
 * This function check whether the given sprite is on ground.
 *
 * @param sprite Sprite to check for ground status.
 * @return 1 if on ground, 0 otherwise.
 */
uint8_t Environment::isOnGround(Sprite* sprite) {
	// TODO: If the tile dimension would be a power of 2, it would make things easier

	if((sprite->getPositionY()+sprite->getHeight()) % tileHeight == 0) {
		uint32_t tileStartX = sprite->getPositionX() / tileWidth;
		uint32_t tileEndX = (sprite->getPositionX() + sprite->getWidth()) / tileWidth;
		uint32_t tileY = (sprite->getPositionY() + sprite->getHeight()) / tileHeight;

		for(uint32_t x=tileStartX; x<=tileEndX; x++) {
			if(tileMap[tileY][x] != 0) {
				return 1;
			}
		}
	}
	return 0;
}

/**
 * Try to move the sprite to the desired x,y position.
 *
 * @param sprite Sprite to move
 * @param desiredPositionX Desired position X
 * @param desiredPositionY Desired position Y
 * @return 1 if we were able to move the sprite to the desired position, 0 otherwise.
 */
uint8_t Environment::move(Sprite* sprite, uint32_t desiredPositionX, uint32_t desiredPositionY) {
	uint8_t returnValue = 1;
	uint32_t possiblePositionX = desiredPositionX;
	uint32_t possiblePositionY = desiredPositionY;

	// Horizontal move
	if(desiredPositionX != sprite->getPositionX()) {
		uint32_t tileStartY = sprite->getPositionY() / tileHeight;
		uint32_t tileEndY = (sprite->getPositionY() + sprite->getHeight() - 1) / tileHeight;

		// Moving right
		if(desiredPositionX > sprite->getPositionX()) {
			uint32_t tileX = (desiredPositionX + sprite->getWidth()) / tileWidth;

			for(uint32_t y=tileStartY; y<=tileEndY; y++) {
				if(tileMap[y][tileX] != 0) {
					possiblePositionX -= (desiredPositionX + sprite->getWidth()) % tileWidth;
					returnValue = 0;
					break;
				}
			}
		}
		// Moving left
		else {
			uint32_t tileX = desiredPositionX / tileWidth;

			for(uint32_t y=tileStartY; y<=tileEndY; y++) {
				if(tileMap[y][tileX] != 0) {
					// Moving left
					possiblePositionX += tileWidth - (desiredPositionX % tileWidth);
					returnValue = 0;
					break;
				}
			}
		}

	}
	// Vertical move
	if(desiredPositionY != sprite->getPositionY()) {
		uint32_t tileStartX = sprite->getPositionX() / tileWidth;
		uint32_t tileEndX = (sprite->getPositionX() + sprite->getWidth() -1) / tileWidth;

		// Moving up
		if(desiredPositionY < sprite->getPositionY()) {
			uint32_t tileY = desiredPositionY / tileHeight;

			for(uint32_t x= tileStartX; x<=tileEndX; x++) {
				if(tileMap[tileY][x] != 0) {
					possiblePositionY += tileHeight - (desiredPositionY % tileHeight);
					returnValue = 0;
					break;
				}
			}
		}
		// Moving down
		else {
			uint32_t tileY = (desiredPositionY + sprite->getHeight()) / tileHeight;

			for(uint32_t x= tileStartX; x<=tileEndX; x++) {
				if(tileMap[tileY][x] != 0) {
					possiblePositionY -= (desiredPositionY + sprite->getHeight()) % tileHeight;
					returnValue = 0;
					break;
				}
			}

		}
	}
	sprite->setPosition(possiblePositionX, possiblePositionY);
	return returnValue;
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
		//sprite->setPosition(x,y);
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
void Environment::set(Sprite* hero, uint32_t x, uint32_t y) {
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
	/*uint32_t* lcd_ptr = videoMemory->getPointer();
	uint32_t bufferLength = videoMemory->getWidth()*videoMemory->getHeight();

	for (uint32_t i=0; i<bufferLength; i++) {
		*(lcd_ptr++) = 0x00F7E3BD;
	}*/

	if(background != 0) {
		background->render(videoMemory);
	}
}

void Environment::renderTiles(VideoMemory* videoMemory) {
	uint32_t iRenderStart = visibleArea->y1/tileHeight;
	uint32_t iRenderStop = visibleArea->y2/tileHeight;
	uint32_t jRenderStart = visibleArea->x1/tileWidth;
	uint32_t jRenderStop = visibleArea->x2/tileWidth;

	// If the division is not exact, we render one more tile
	// This is similar to the ceiling mathematical function
	if((visibleArea->y2%tileHeight) != 0) {
		iRenderStop++;
	}
	if((visibleArea->x2%tileWidth) != 0) {
		jRenderStop++;
	}


	for(uint32_t i=iRenderStart; i<iRenderStop; i++) {
		for(uint32_t j=jRenderStart; j<jRenderStop; j++) {
			// Set the tile position - This is subject to change
			if(tileMap[i][j] != 0) {
				tileMap[i][j]->setPosition(j*tileWidth, i*tileHeight);
				tileMap[i][j]->render(videoMemory);
			}
		}
	}
}

void Environment::renderHero(VideoMemory* videoMemory) {
	hero->render(videoMemory);
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
	hero->update();
}

void Environment::updateSprites() {

}
