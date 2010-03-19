/*
 * VisibleArea.h
 *
 *  Created on: Mar 14, 2010
 *      Author: fgervais
 */

#ifndef VISIBLEAREA_H_
#define VISIBLEAREA_H_

#include <stdint.h>

class VisibleArea {
public:
	VisibleArea();
	virtual ~VisibleArea();

	uint32_t x1;
	uint32_t y1;
	uint32_t x2;
	uint32_t y2;
};

#endif /* VISIBLEAREA_H_ */
