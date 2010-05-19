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
#include "LCDConfiguration.h"

BlueScreenSunLCD::BlueScreenSunLCD(LCD_Typedef* lcdRegisters)
	: LCDControllerDriver(lcdRegisters) {

	this->lcdRegisters = lcdRegisters;

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

void BlueScreenSunLCD::configure(LCDConfiguration config) {
	uint32_t regVal;

	if(getState() == Up) {
		powerDown();
	}

	regVal = lcdRegisters->LCD_TIMH;
	regVal &= 0x000000FF;
	regVal |= 40 << 8;	//horizontal sync pulse width
	regVal |= 1 << 16;	//horizontal front porch
	regVal |= 1 << 24;	//horizontal back porch
	lcdRegisters->LCD_TIMH = regVal;

	regVal = lcdRegisters->LCD_TIMV;
	regVal &= 0x000003FF;
	regVal |= 9 << 10;	//vertical sync pulse width
	regVal |= 1 << 16;	//vertical front porch
	regVal |= 1 << 24;	//vertical back porch
	lcdRegisters->LCD_TIMV = regVal;

	// Call Base class function
	LCDControllerDriver::configure(config);

	if(getState() == Down) {
		powerUp();
	}

	// The example has 1s delay here
	LPC2478::delay(50000);	// 50 ms

	clearScreen();
}
