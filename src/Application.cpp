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

int main() {

	Gpio *gpioA = LPC2478::getGpio0();

	// Set default port behavior
	GpioConfiguration portConfig(Gpio::INPUT);
	gpioA->configure(portConfig);

	// Configure blinking led
	GpioPinConfiguration ledPinConfig;
	ledPinConfig.pin = Gpio::OUTPUT;
	gpioA->getPin(0)->configure(ledPinConfig);

	GpioPin *led = gpioA->getPin(0);

	// Blink led
	while(1) {
		led->setHigh();	// On
		for(uint32_t i=0; i<100000; i++);
		led->setLow();	// Off
		for(uint32_t i=0; i<100000; i++);
	}

}
