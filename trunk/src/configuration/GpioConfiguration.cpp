/*
 * GpioConfiguration.cpp
 *
 *  Created on: Jan 6, 2010
 *      Author: fgervais
 */

#include "GpioConfiguration.h"

GpioConfiguration::GpioConfiguration(uint8_t portLength) {
	pin = new uint8_t[portLength];
}

GpioConfiguration::GpioConfiguration(uint8_t portLength, uint8_t defaultConfig) {
	pin = new uint8_t[portLength];

	// Initialize array
	for(uint8_t i=0; i<portLength; i++) {
		pin[i] = defaultConfig;
	}
}

GpioConfiguration::~GpioConfiguration() {
	// Free memory
	delete[] pin;
}
