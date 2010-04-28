/*
 * Background.h
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "Renderable.h"

#include <stdint.h>

class Bitmap;
class Environment;
class VideoMemory;

struct DMACCxLLI;

class Background : public Renderable {
public:
	Background(Bitmap* handle, uint32_t width, uint32_t height, Environment* environment);
	virtual ~Background();

	Bitmap* getHandle() { return bitmap; }
	uint32_t getHeight() { return height; }
	uint32_t getWidth() { return width; }

	// Interface implementation
	virtual void render(VideoMemory*);

	void set(Environment* environment);

private:
	Bitmap* bitmap;
	uint32_t height;
	uint32_t width;

	Environment* environment;

	DMACCxLLI** lli;
};

#endif /* BACKGROUND_H_ */
