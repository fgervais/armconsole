/*
 * BlueScreenSunLCD.h
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#ifndef BLUESCREENSUNLCD_H_
#define BLUESCREENSUNLCD_H_

#include "LCDControllerDriver.h"

class GpioPin;

/**
 * This is a specific implementation of the LCD controller
 * for the BlueScreen SUN development board.
 *
 * This board use some special things to drive the LCD
 * like an IO pin to drive the LED_K pin of the display.
 *
 * @see www.thaieasyelec.com
 */
class BlueScreenSunLCD : public LCDControllerDriver {
public:
	BlueScreenSunLCD(LCD_Typedef* lcdRegisters);
	virtual ~BlueScreenSunLCD();

	virtual void powerUp();
	virtual void powerDown();

private:
	GpioPin* lcdAdjPin;
};

#endif /* BLUESCREENSUNLCD_H_ */
