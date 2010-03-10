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
	void add(Sprite sprite, uint32_t x, uint32_t y);
	void add(Tile tile, uint32_t x, uint32_t y);
	void set(Hero hero, uint32_t x, uint32_t y);
	void set(Background* background);
	void set(Physics physics);

private:
	uint32_t height;
	uint32_t width;
	uint32_t heightInTile;
	uint32_t widthInTile;

	Tile*** tileMap;
	Sprite** sprite;
	Hero* hero;
	Background* background;
	Physics* physics;
};

#endif /* ENVIRONMENT_H_ */
