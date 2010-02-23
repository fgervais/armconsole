/*
 * LCDConfiguration.h
 *
 *  Created on: Feb 22, 2010
 *      Author: fgervais
 */

#ifndef LCDCONFIGURATION_H_
#define LCDCONFIGURATION_H_

#include <stdint.h>

class LCDConfiguration {
public:
	LCDConfiguration();
	virtual ~LCDConfiguration();

	uint32_t width;
	uint32_t height;
	uint32_t bufferBaseAddress;
};

#endif /* LCDCONFIGURATION_H_ */
