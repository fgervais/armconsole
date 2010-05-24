/*
 * Environment.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Environment.h"
#include "Sprite.h"
#include "Tile.h"
#include "Megaman.h"
#include "Background.h"
#include "Physics.h"
#include "VisibleArea.h"
#include "Debug.h"
#include "VideoMemory.h"

// Posted by TastyWheat on LinuxQuestions.org
// This will work for positive number only
#define CEILING(X) ((X-(uint32_t)(X)) > 0 ? (uint32_t)(X+1) : (uint32_t)(X))

Environment::Environment(uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight) {
	this->height = height;
	this->width = width;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;

	float heightInTile_t = (float)height/tileHeight;
	heightInTile = CEILING(heightInTile_t);
	// This is needed because we need upper integer round-up
	/*if((height % tileHeight) > 0) {
		heightInTile++;
	}*/

	float widthInTile_t = (float)width/tileWidth;
	widthInTile = CEILING(widthInTile_t);
	/*if((width % tileWidth) > 0) {
		widthInTile++;
	}*/

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

	spriteMap = new SpriteContainer**[heightInTile];
	for(uint32_t i=0; i<widthInTile; i++) {
		spriteMap[i] = new SpriteContainer*[widthInTile];
	}

	// Initialize the tile array
	for(uint32_t i=0; i<heightInTile; i++) {
		for(uint32_t j=0; j<widthInTile; j++) {
			spriteMap[i][j] = 0;
		}
	}

	// Sprite limit without the Hero
	// TODO: Should the spriteLimit be stored somewhere else?
	spriteLimit = 32;

	activeSprite = new SpriteContainer*[spriteLimit];
	for(uint32_t i=0; i<spriteLimit; i++) {
		activeSprite[i] = 0;
	}

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
	// Update the visible area to follow the hero
	int32_t heroMiddlePosition = hero->getPositionX() + (hero->getWidth() / 2);

	visibleArea->x = heroMiddlePosition - (visibleArea->width/2);

	if((heroMiddlePosition - (int32_t)(visibleArea->width/2)) < 0) {
		visibleArea->x = 0;
	}
	else if((heroMiddlePosition + (int32_t)(visibleArea->width/2)) > (int32_t)width) {
		visibleArea->x = width - visibleArea->width;
	}

	// Activate sprites if needed
	activateSprites();

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

void Environment::checkCollision(Sprite* sprite) {
	uint32_t left1, left2;
	uint32_t right1, right2;
	uint32_t top1, top2;
	uint32_t bottom1, bottom2;

	for(uint32_t activeSpriteIterator=0; activeSpriteIterator<spriteLimit; activeSpriteIterator++) {
		if(activeSprite[activeSpriteIterator] != 0) {
			// Do not check a sprite with itself
			if(sprite != activeSprite[activeSpriteIterator]->sprite) {

				// This algorithme is comming from gamedev.net
				left1 = sprite->getPositionX();
				left2 = activeSprite[activeSpriteIterator]->sprite->getPositionX();
				right1 = left1 + sprite->getWidth();
				right2 = left2 + activeSprite[activeSpriteIterator]->sprite->getWidth();
				top1 = sprite->getPositionY();
				top2 = activeSprite[activeSpriteIterator]->sprite->getPositionY();
				bottom1 = top1 + sprite->getHeight();
				bottom2 = top2 + activeSprite[activeSpriteIterator]->sprite->getHeight();

				if (bottom1 < top2) continue;
				if (top1 > bottom2) continue;

				if (right1 < left2) continue;
				if (left1 > right2) continue;

				// Both sprite should be aware of the collision
				//sprite->collideWith(activeSprite[activeSpriteIterator]->sprite);
				//activeSprite[activeSpriteIterator]->sprite->collideWith(sprite);

				// TODO: Remove me and use the two upper line.
				// This is for demo purpose only
				activeSprite[activeSpriteIterator]->active = 0;
				activeSprite[activeSpriteIterator] = 0;
			}
		}
	}
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
 * @return 1 if the sprite is successfully added. 0 otherwise.
 */
uint8_t Environment::add(Sprite* sprite, uint32_t x, uint32_t y) {
	if(x < width && y < height) {
		SpriteContainer* container = new SpriteContainer();
		// Add some more information to the sprite so we can spawn it
		// again and again in the same area
		container->sprite = sprite;
		container->active = 0;
		container->spawnPositionX = x;
		container->spawnPositionY = y;

		spriteMap[y/tileHeight][x/tileWidth] = container;
		return 1;
	}
	return 0;
}

/**
 * Add a tile to the tile map.
 *
 * @param tile Tile to be added
 * @param x X position of the tile in the tile map
 * @param y Y position of the tile in the tile map
 * @return 1 if the tile is successfully added. 0 otherwise.
 */
uint8_t Environment::add(Tile* tile, uint32_t x, uint32_t y) {
	if(x < widthInTile && y < heightInTile) {
		tileMap[y][x] = tile;
		return 1;
	}
	return 0;
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
	uint32_t iRenderStart = visibleArea->y/tileHeight;
	//float iRenderStop_t = (float)(visibleArea->y+visibleArea->height)/tileHeight;
	//uint32_t iRenderStop = CEILING(iRenderStop_t);
	uint32_t iRenderStop = (visibleArea->y+visibleArea->height-1)/tileHeight;
	uint32_t jRenderStart = visibleArea->x/tileWidth;
	//float jRenderStop_t = (float)(visibleArea->x+visibleArea->width)/tileWidth;
	//uint32_t jRenderStop = CEILING(jRenderStop_t);
	uint32_t jRenderStop = (visibleArea->x+visibleArea->width-1)/tileWidth;

	// If the division is not exact, we render one more tile
	// This is similar to the ceiling mathematical function
	/*if(((visibleArea->y+visibleArea->height)%tileHeight) != 0) {
		iRenderStop++;
	}
	if(((visibleArea->x+visibleArea->width)%tileWidth) != 0) {
		jRenderStop++;
	}*/


	for(uint32_t i=iRenderStart; i<=iRenderStop; i++) {
		for(uint32_t j=jRenderStart; j<=jRenderStop; j++) {
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
	for(uint32_t i=0; i<spriteLimit; i++) {
		if(activeSprite[i] != 0) {
			if((activeSprite[i]->sprite->getPositionX()+activeSprite[i]->sprite->getWidth()) > visibleArea->x && activeSprite[i]->sprite->getPositionX() < (visibleArea->x + visibleArea->width)) {
				if((activeSprite[i]->sprite->getPositionY()+activeSprite[i]->sprite->getHeight()) > visibleArea->y && activeSprite[i]->sprite->getPositionY() < (visibleArea->y + visibleArea->height)) {

					activeSprite[i]->sprite->render(videoMemory);
				}
			}
		}
	}
}

void Environment::updateBackground() {

}

void Environment::updateTiles() {
	for(uint32_t i=visibleArea->y/tileHeight; i<(visibleArea->y+visibleArea->height)/tileHeight; i++) {
		for(uint32_t j=visibleArea->x/tileWidth; j<(visibleArea->x+visibleArea->width)/tileWidth; j++) {
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
	for(uint32_t i=0; i<spriteLimit; i++) {
		if(activeSprite[i] != 0) {
			// If the sprite is inside the visible area, update it
			if((activeSprite[i]->sprite->getPositionX()+activeSprite[i]->sprite->getWidth()) > visibleArea->x && activeSprite[i]->sprite->getPositionX() < (visibleArea->x + visibleArea->width)) {
				if((activeSprite[i]->sprite->getPositionY()+activeSprite[i]->sprite->getHeight()) > visibleArea->y && activeSprite[i]->sprite->getPositionY() < (visibleArea->y + visibleArea->height)) {
					activeSprite[i]->sprite->update();
				}
			}
			// If not, remove it from the active sprite
			else if(activeSprite[i]->sprite->getPositionX() != activeSprite[i]->spawnPositionX || activeSprite[i]->sprite->getPositionY() != activeSprite[i]->spawnPositionY) {
				activeSprite[i]->active = 0;
				activeSprite[i] = 0;
			}
		}
	}
}

void Environment::activateSprites() {
	/*
	 * Sprites needs to be activated if their location is on the closest tile outside the
	 * visible area.
	 *
	 * Like this :
	 *[x] x x x x x x x x x[x]
	 * x +----------------+ x
	 * x | Visible area   | x
	 * x |                | x
	 * x +----------------+ x
	 *[x] x x x x x x x x x[x]
	 *
	 * If there is a sprite where the Xs are, it will be activated. This is the way SNES is doing
	 * Well in Megaman X at least.
	 *
	 * Note: I won't include the corner of the visible area. It's way faster and I don't think
	 * it'll cause any problem. If I'm wrong, it could be fixed easily.
	 */
	uint32_t visibleAreaInTileY1 = visibleArea->y/tileHeight;
	uint32_t visibleAreaInTileY2 = (visibleArea->y+visibleArea->height-1)/tileHeight;
	uint32_t visibleAreaInTileX1 = visibleArea->x/tileWidth;
	uint32_t visibleAreaInTileX2 = (visibleArea->x+visibleArea->width-1)/tileWidth;

	//Since there is no horizontal scrolling for now, this is not needed
	/*
	// Upper line
	if(hero->getPositionY() < 0) {
		if(visibleAreaInTileY1 > 0) {
			for(uint32_t j=visibleAreaInTileX1; j<=visibleAreaInTileX2; j++) {
				if(spriteMap[visibleAreaInTileY1-1][j] != 0) {
					activate(spriteMap[visibleAreaInTileY1-1][j]);
				}
			}
		}
	}
	// Lower line
	if(hero->getVelocityY() > 0) {
		if(visibleAreaInTileY2 < (heightInTile-1)) {
			for(uint32_t j=visibleAreaInTileX1; j<=visibleAreaInTileX2; j++) {
				if(spriteMap[visibleAreaInTileY2+1][j] != 0) {
					activate(spriteMap[visibleAreaInTileY2+1][j]);
				}
			}
		}
	}
	*/
	// Left side line
	if(hero->getVelocityX() < 0) {
		if(visibleAreaInTileX1 > 0) {
			for(uint32_t i=visibleAreaInTileY1; i<=visibleAreaInTileY2; i++) {
				if(spriteMap[i][visibleAreaInTileX1-1] != 0 && spriteMap[i][visibleAreaInTileX1-1]->active == 0) {
					activate(spriteMap[i][visibleAreaInTileX1-1]);
				}
			}
		}
	}
	// Right side line
	if(hero->getVelocityX() > 0) {
		if(visibleAreaInTileX2 < (widthInTile-1)) {
			for(uint32_t i=visibleAreaInTileY1; i<=visibleAreaInTileY2; i++) {
				if(spriteMap[i][visibleAreaInTileX2+1] != 0 && spriteMap[i][visibleAreaInTileX2+1]->active == 0) {
					activate(spriteMap[i][visibleAreaInTileX2+1]);
				}
			}
		}
	}
}

uint8_t Environment::activate(SpriteContainer* container) {
	container->sprite->setPosition(container->spawnPositionX, container->spawnPositionY);
	// Add sprite to the active sprite list if space remains
	for(uint32_t activeSpriteIterator=0; activeSpriteIterator<spriteLimit; activeSpriteIterator++) {
		if(activeSprite[activeSpriteIterator] == 0) {
			activeSprite[activeSpriteIterator] = container;
			container->active = 1;
			return 1;
		}
	}
	return 0;
}
