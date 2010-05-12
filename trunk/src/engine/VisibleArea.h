/*
 * VisibleArea.h
 *
 *  Created on: Mar 14, 2010
 *      Author: fgervais
 */

#ifndef VISIBLEAREA_H_
#define VISIBLEAREA_H_

#include <stdint.h>

/**
 * This is the portion of the game that we see on the screen.
 *
 * All those coordinates are in pixel unit.
 */
class VisibleArea {
public:
	VisibleArea();
	virtual ~VisibleArea();

	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
};

#endif /* VISIBLEAREA_H_ */
