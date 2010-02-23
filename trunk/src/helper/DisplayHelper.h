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

	void drawLine(uint32_t x, uint32_t y);

	void println(uint32_t x, uint32_t y);

private:
	LCDControllerDriver* lcd;
};

#endif /* DISPLAYHELPER_H_ */
