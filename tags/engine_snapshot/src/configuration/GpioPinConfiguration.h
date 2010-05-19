/*
 * GpioPinConfiguration.h
 *
 *  Created on: Jan 6, 2010
 *      Author: fgervais
 */

#ifndef GPIOPINCONFIGURATION_H_
#define GPIOPINCONFIGURATION_H_

#include "stdint.h"

class GpioPinConfiguration {
public:
	GpioPinConfiguration();
	virtual ~GpioPinConfiguration();

	uint8_t pin;
};

#endif /* GPIOPINCONFIGURATION_H_ */
