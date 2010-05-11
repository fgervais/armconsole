/*
 * Environment.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Synchronized.h"
#include "Renderable.h"
#include <stdint.h>

class Sprite;
class Tile;
class Hero;
class VideoMemory;
class Background;
class Physics;
class VisibleArea;
class Request;

class Environment : public Synchronized, public Renderable {
public:
	Environment(uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight);
	virtual ~Environment();

	// Abstract functions
	virtual void build() = 0;

	// Renderable interface implementation
	virtual void render(VideoMemory* videoMemory);
	// Synchronized interface implementation
	virtual void update();

	uint8_t isOnGround(Sprite*);
	uint8_t isOnWall(Sprite*) { return 0; }
	uint8_t isReachable(Sprite*, uint32_t x, uint32_t y) { return 0; }
	uint8_t move(Sprite*, uint32_t desiredPositionX, uint32_t desiredPositionY);

	VisibleArea* getVisibleArea() { return visibleArea; };
	Sprite* getHero() { return hero; }

protected:
	uint8_t add(Sprite* sprite, uint32_t x, uint32_t y);
	uint8_t add(Tile* tile, uint32_t x, uint32_t y);
	void set(Sprite* hero, uint32_t x, uint32_t y);
	void set(Background* background);
	void set(Physics* physics);
	void set(VisibleArea* visibleArea);

private:
	// Coming from constructor
	uint32_t height;
	uint32_t width;
	uint32_t tileHeight;
	uint32_t tileWidth;
	uint32_t heightInTile;
	uint32_t widthInTile;

	Tile*** tileMap;
	Sprite** sprites;
	uint32_t spriteLimit;
	uint32_t numberOfSprite;
	Sprite* hero;
	Background* background;
	Physics* physics;
	VisibleArea* visibleArea;

	void renderBackground(VideoMemory* videoMemory);
	void renderTiles(VideoMemory* videoMemory);
	void renderHero(VideoMemory* videoMemory);
	void renderSprites(VideoMemory* videoMemory);

	void updateBackground();
	void updateTiles();
	void updateHero();
	void updateSprites();
};

#endif /* ENVIRONMENT_H_ */
