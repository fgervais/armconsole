/*
 * LPC2478.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef LPC2478_H_
#define LPC2478_H_

#include "Microcontroller.h"

class Gpio;
class HostControllerDriver;

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
	static HostControllerDriver* getHCD();

private:
	static Gpio *gpio0;
	static HostControllerDriver *hcd;
};

#endif /* LPC2478_H_ */
