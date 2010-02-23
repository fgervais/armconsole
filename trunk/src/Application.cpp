/*
 * LPC2478.h
 *
 *  Created on: Dec 31, 2009
 *      Author: fgervais
 */

#include "GpioConfiguration.h"
#include "GpioPinConfiguration.h"
#include "Gpio.h"
#include "GpioPin.h"
#include "LPC2478.h"
#include "HostControllerDriver.h"
#include "LCDControllerDriver.h"
#include "LCDConfiguration.h"

#include "irq.h"
#include "swi.h"

int main() {
	Gpio *gpio1 = LPC2478::getGpio1();

	// Set default port behavior
	GpioConfiguration portConfig(32, Gpio::INPUT);
	gpio1->configure(portConfig);

	// Configure blinking led
	GpioPinConfiguration *ledPinConfig = new GpioPinConfiguration();
	ledPinConfig->pin = Gpio::OUTPUT;
	gpio1->getPin(12)->configure(*ledPinConfig);

	GpioPin *led = gpio1->getPin(12);

	// USB debug section
	//HostControllerDriver* hcd = LPC2478::getHCD();
	//IntEnable();

	LCDConfiguration lcdConfig;
	lcdConfig.bufferBaseAddress = 0xA0000000;
	lcdConfig.height = 272;
	lcdConfig.width = 480;
	LCDControllerDriver* lcd = LPC2478::getLCD();
	lcd->configure(lcdConfig);

	// Blink led
	while(1) {
		led->setHigh();	// On
		for(uint32_t i=0; i<100000; i++);
		led->setLow();	// Off
		for(uint32_t i=0; i<100000; i++);
	}

}
