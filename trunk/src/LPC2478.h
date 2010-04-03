/*
 * LPC2478.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef LPC2478_H_
#define LPC2478_H_

#include "Microcontroller.h"

#include <stdint.h>

class Gpio;
class HostControllerDriver;
class LCDControllerDriver;
class DAC;

/**
 * Software interface to the LPC2478 microcontroller.
 *
 * This is the class to ask for any peripheral of the chip.
 * It has static function so it can be reached from anywhere.
 */
class LPC2478: public Microcontroller {
public:
	LPC2478();
	virtual ~LPC2478();

	static Gpio* getGpio0();
	static Gpio* getGpio1();
	static Gpio* getGpio2();
	static Gpio* getGpio3();
	static HostControllerDriver* getHCD();
	static LCDControllerDriver* getLCD();
	static DAC* getDAC();

	static void delay(uint32_t usec);
private:
	static Gpio *gpio0;
	static Gpio *gpio1;
	static Gpio *gpio2;
	static Gpio *gpio3;
	static HostControllerDriver *hcd;
	static LCDControllerDriver *lcd;
	static DAC* dac;
};

#endif /* LPC2478_H_ */
