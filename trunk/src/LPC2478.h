/*
 * LPC2478.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef LPC2478_H_
#define LPC2478_H_

#include "Microcontroller.h"
#include "target.h"

#include <stdint.h>

class Gpio;
class HostControllerDriver;
class LCDControllerDriver;
class DAC;
class DMAChannel;
class Timer;

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

	static uint32_t getFrequency() { return frequency; };
	static Gpio* getGpio0();
	static Gpio* getGpio1();
	static Gpio* getGpio2();
	static Gpio* getGpio3();
	static HostControllerDriver* getHCD();
	static LCDControllerDriver* getLCD();
	static DAC* getDAC();
	static DMAChannel* getDMA0();
	static DMAChannel* getDMA1();
	static Timer* getTimer0();
	static Timer* getTimer1();
	static Timer* getTimer2();
	static Timer* getTimer3();

	static void delay(uint32_t usec);
private:
	static const uint32_t frequency = Fcclk;

	static Gpio* gpio0;
	static Gpio* gpio1;
	static Gpio* gpio2;
	static Gpio* gpio3;
	static HostControllerDriver* hcd;
	static LCDControllerDriver* lcd;
	static DAC* dac;
	static DMAChannel* dma0;
	static DMAChannel* dma1;
	static Timer* timer0;
	static Timer* timer1;
	static Timer* timer2;
	static Timer* timer3;
};

#endif /* LPC2478_H_ */
