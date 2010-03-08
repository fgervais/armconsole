/*
 * Environment.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <stdint.h>

class Sprite;
class Tile;

class Environment {
public:
	Environment(uint32_t height, uint32_t width);
	virtual ~Environment();

	virtual void build() = 0;

	void add(Sprite sprite, uint32_t x, uint32_t y);
	void add(Tile tile, uint32_t x, uint32_t y);

private:
	uint32_t height;
	uint32_t width;

	uint32_t** tileMap;
	uint32_t** spriteMap;
};

#endif /* ENVIRONMENT_H_ */
