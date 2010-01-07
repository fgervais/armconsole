/*
 * GpioPin.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include "LPC23xx.h"
#include "Vector.h"

#include <stdint.h>

class GpioPinEventListener;
class GpioPinConfiguration;

/**
 * General purpose IO pin.
 */
class GpioPin {
public:
	GpioPin(FIO_TypeDef *gpioRegisters,
			uint8_t pinNumber, uint8_t portNumber);
	virtual ~GpioPin();
	void extInterrupt();
	void configure(GpioPinConfiguration config);
	uint32_t isHigh();
	void setHigh();
	void setLow();
	void addEventListener(GpioPinEventListener *listener);
	uint8_t getPinNumber() { return pinNumber; }
	uint8_t getPortNumber() { return portNumber; }

private:
	uint8_t pinNumber;
	uint32_t portNumber;
	FIO_TypeDef *gpioRegisters;
	// TODO: This vector thing is buggy
	//Vector<GpioPinEventListener*> listeners;
	GpioPinEventListener* listener;
	uint8_t extiConfigured;

	void configureInterrupt();
};

#endif /* GPIOPIN_H_ */
