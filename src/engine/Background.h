/*
 * Background.h
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <stdint.h>

class Background {
public:
	Background(uint32_t* handle, uint32_t height, uint32_t width);
	virtual ~Background();

	uint32_t* getHandle() { return handle; }
	uint32_t getHeight() { return height; }
	uint32_t getWidth() { return width; }

private:
	uint32_t* handle;
	uint32_t height;
	uint32_t width;
};

#endif /* BACKGROUND_H_ */
