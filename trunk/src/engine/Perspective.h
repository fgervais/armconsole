/*
 * Perspective.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef PERSPECTIVE_H_
#define PERSPECTIVE_H_

#include <stdint.h>

class Perspective {
public:
	Perspective();
	virtual ~Perspective();

	uint32_t x1;
	uint32_t y1;
	uint32_t x2;
	uint32_t y2;
};

#endif /* PERSPECTIVE_H_ */
