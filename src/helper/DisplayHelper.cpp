/*
 * DisplayHelper.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#include "DisplayHelper.h"
#include "LCDControllerDriver.h"

DisplayHelper::DisplayHelper(LCDControllerDriver* lcd) {
	this->lcd = lcd;
}

DisplayHelper::~DisplayHelper() {

}

void DisplayHelper::drawImage(uint32_t x, uint32_t y, uint32_t* imageBase, uint32_t imageWidth, uint32_t imageHeight) {
	uint32_t nrows = lcd->getHeight();
	uint32_t ncols = lcd->getWidth();
	uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();

	if(x < ncols && y < nrows) {

		for (uint32_t i=0; i<imageHeight; i++) {
			for (uint32_t j=0; j<imageWidth; j++) {
				lcd_ptr[(i+y)*ncols + (j+x)] = imageBase[i*imageWidth + j];

			}
		}

	}
}
