/*
 * VideoMemory.h
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#ifndef VIDEOMEMORY_H_
#define VIDEOMEMORY_H_

#include <stdint.h>

class VideoMemory {
public:
	VideoMemory(uint32_t* pointer, uint32_t height, uint32_t width);
	virtual ~VideoMemory();

	uint32_t* getPointer() { return pointer; };
	uint32_t getHeight() { return height; };
	uint32_t getWidth() { return width; };
private:
	uint32_t* pointer;
	uint32_t height;
	uint32_t width;
};

#endif /* VIDEOMEMORY_H_ */
