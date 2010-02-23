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

	virtual void powerUp();
	virtual void powerDown();
	void configure(LCDConfiguration config);

private:
	enum State { Up, Down };

	LCD_Typedef* lcdRegisters;
	State lcdState;
};

#endif /* LCDCONTROLLERDRIVER_H_ */
