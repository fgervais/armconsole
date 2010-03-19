/*
 * Tile.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef TILE_H_
#define TILE_H_

#include "Graphic.h"

class Environment;
class VideoMemory;

class Tile: public Graphic {
public:
	Tile(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame);
	virtual ~Tile();
private:

};

#endif /* TILE_H_ */
