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

class Graphic : public Synchronized, public Renderable {
public:
	Graphic();
	virtual ~Graphic();

	virtual void update();
	virtual void render();

private:
	uint32_t height;
	uint32_t width;
	uint32_t** graphics;
	uint32_t numFrame;
};

#endif /* GRAPHIC_H_ */
