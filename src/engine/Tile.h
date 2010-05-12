/*
 * Tile.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef TILE_H_
#define TILE_H_

#include "Synchronized.h"
#include "Renderable.h"
#include <stdint.h>

class Environment;
class VideoMemory;
class Bitmap;

class Tile : public Synchronized, public Renderable {
public:
	Tile(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame, Environment* environment);
	virtual ~Tile();

	// Interface implementation
	virtual void update();
	virtual void render(VideoMemory*);

	void setPosition(uint32_t x, uint32_t y);
	void set(Environment* environment);
private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
	uint32_t numberOfFrame;
	uint32_t currentFrame;
	uint32_t positionX;
	uint32_t positionY;
	Environment* environment;
	uint32_t friction;
};

#endif /* TILE_H_ */
