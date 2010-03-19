/*
 * Graphic.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "Synchronized.h"
#include "Renderable.h"
#include <stdint.h>

class VideoMemory;
class Environment;

class Graphic : public Synchronized, public Renderable {
public:
	Graphic(uint32_t height, uint32_t width, uint32_t** handles, uint32_t numberOfFrame);
	virtual ~Graphic();

	// Interface implementation
	virtual void update();
	virtual void render(VideoMemory*);

	void setPosition(uint32_t x, uint32_t y);
	void set(Environment* environment);

	//uint32_t getPositionX() { return xPosition; };
	//uint32_t getPositionY() { return yPosition; };

private:
	uint32_t height;
	uint32_t width;
	uint32_t** handles;
	uint32_t numberOfFrame;
	uint32_t currentFrame;
	uint32_t positionX;
	uint32_t positionY;
	Environment* environment;
};

#endif /* GRAPHIC_H_ */
