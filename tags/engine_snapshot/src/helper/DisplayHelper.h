/*
 * DisplayHelper.h
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#ifndef DISPLAYHELPER_H_
#define DISPLAYHELPER_H_

#include <stdint.h>

class LCDControllerDriver;

class DisplayHelper {
public:
	DisplayHelper(LCDControllerDriver* lcd);
	virtual ~DisplayHelper();

	void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
	void drawImage(uint32_t x, uint32_t y, uint32_t* imageBase, uint32_t imageWidth, uint32_t imageHeight);
	void println(uint32_t x, uint32_t y);
	void drawString(uint32_t x, uint32_t y, char* string);
	void drawString(uint32_t x, uint32_t y, char* string, uint32_t fontColor, uint32_t bgColor);

private:
	LCDControllerDriver* lcd;
};

#endif /* DISPLAYHELPER_H_ */
