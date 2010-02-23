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

	lcdAdjPin = LPC2478::getGpio2()->getPin(1);

	GpioPinConfiguration pinConfig;
	pinConfig.pin = Gpio::OUTPUT;

	lcdAdjPin->configure(pinConfig);
	lcdAdjPin->setLow();
}

BlueScreenSunLCD::~BlueScreenSunLCD() {

}

void BlueScreenSunLCD::powerUp() {
	LCDControllerDriver::powerUp();
	lcdAdjPin->setHigh();
}

void BlueScreenSunLCD::powerDown() {
	lcdAdjPin->setLow();
	LCDControllerDriver::powerDown();
}
