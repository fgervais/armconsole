/*
 * GpioConfiguration.h
 *
 *  Created on: Jan 6, 2010
 *      Author: fgervais
 */

#ifndef GPIOCONFIGURATION_H_
#define GPIOCONFIGURATION_H_

#include <stdint.h>

class GpioConfiguration {
public:
	GpioConfiguration(uint8_t portLength);
	GpioConfiguration(uint8_t portLength, uint8_t defaultConfig);
	virtual ~GpioConfiguration();

	uint8_t* pin;
};

#endif /* GPIOCONFIGURATION_H_ */
