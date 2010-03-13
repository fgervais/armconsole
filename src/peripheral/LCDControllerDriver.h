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
#include "Peripheral.h"
#include <stdint.h>

class LCDControllerDriver : public Peripheral {
public:
	LCDControllerDriver(LCD_Typedef* lcdRegisters);
	virtual ~LCDControllerDriver();

	enum State { Up, Down };

	virtual void powerUp();
	virtual void powerDown();
	virtual void configure(LCDConfiguration config);
	void clearScreen();
	void setBackground(uint32_t rgbColor);

	State getState() { return lcdState; }
	uint32_t getBufferBase() { return bufferBase; }
	uint32_t getHeight() { return lcdHeight; }
	uint32_t getWidth() { return lcdWidth; }

private:
	LCD_Typedef* lcdRegisters;
	State lcdState;

	uint32_t bufferBase;
	uint32_t lcdHeight;
	uint32_t lcdWidth;
};

#endif /* LCDCONTROLLERDRIVER_H_ */
