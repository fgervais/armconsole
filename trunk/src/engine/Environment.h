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
class Perspective;

class Environment : public Synchronized, public Renderable {
public:
	Environment(uint32_t heightInTile, uint32_t widthInTile, uint32_t tileHeight, uint32_t tileWidth);
	virtual ~Environment();

	// Abstract functions
	virtual void build() = 0;

	// Renderable interface implementation
	virtual void render(VideoMemory* videoMemory);
	// Synchronized interface implementation
	virtual void update();

protected:
	uint8_t add(Sprite* sprite, uint32_t x, uint32_t y);
	uint8_t add(Tile* tile, uint32_t x, uint32_t y);
	uint8_t set(Hero* hero, uint32_t x, uint32_t y);
	uint8_t set(Background* background);
	uint8_t set(Physics* physics);
	uint8_t set(Perspective* perspective);

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
	Hero* hero;
	Background* background;
	Physics* physics;
	Perspective* perspective;

	void scrollUp(Background* background, uint32_t numberOfPixel);
	void scrollDown(Background* background, uint32_t numberOfPixel);
	void scrollLeft(Background* background, uint32_t numberOfPixel);
	void scrollRight(Background* background, uint32_t numberOfPixel);
};

#endif /* ENVIRONMENT_H_ */
