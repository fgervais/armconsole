/*
 * LCDControllerDriver.h
 *
 *  Created on: Feb 21, 2010
 *      Author: fgervais
 */

#ifndef LCDCONTROLLERDRIVER_H_
#define LCDCONTROLLERDRIVER_H_

class LCDConfiguration;

#include "LPC23xx.h"

class LCDControllerDriver {
public:
	LCDControllerDriver(LCD_Typedef* lcdRegisters);
	virtual ~LCDControllerDriver();

	void configure(LCDConfiguration config);

private:
	LCD_Typedef* lcdRegisters;

	void init();
};

#endif /* LCDCONTROLLERDRIVER_H_ */
