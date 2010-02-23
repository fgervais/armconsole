/*
 * BlueScreenSunLCD.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#include "BlueScreenSunLCD.h"
#include "LPC2478.h"
#include "GpioPin.h"
#include "Gpio.h"
#include "GpioPinConfiguration.h"

BlueScreenSunLCD::BlueScreenSunLCD(LCD_Typedef* lcdRegisters)
	: LCDControllerDriver(lcdRegisters) {

	// This pin is used to control the LCD back light
	lcdAdjPin = LPC2478::getGpio2()->getPin(1);

	GpioPinConfiguration pinConfig;
	pinConfig.pin = Gpio::OUTPUT;

	lcdAdjPin->configure(pinConfig);
	lcdAdjPin->setLow();	// Turn off the back light
}

BlueScreenSunLCD::~BlueScreenSunLCD() {

}

void BlueScreenSunLCD::powerUp() {
	LCDControllerDriver::powerUp();
	lcdAdjPin->setHigh();	// Back light ON
}

void BlueScreenSunLCD::powerDown() {
	lcdAdjPin->setLow();	// Back light OFF
	LCDControllerDriver::powerDown();
}
