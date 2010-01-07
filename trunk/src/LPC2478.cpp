/*
 * LPC2478.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "LPC2478.h"
#include "Gpio.h"

/* Static init. Required to make the compiler happy */
Gpio* LPC2478::gpio0 = 0;

LPC2478::LPC2478() {


}

LPC2478::~LPC2478() {

}

Gpio* LPC2478::getGpio0() {
	if(gpio0 == 0) {
		gpio0 = new Gpio(FIO0, 32, 0);
	}
	return gpio0;
}
