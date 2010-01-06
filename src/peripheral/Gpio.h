/*
 * Gpio.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "Peripheral.h"

class Gpio: public Peripheral {
public:
	Gpio();
	virtual ~Gpio();
};

#endif /* GPIO_H_ */
