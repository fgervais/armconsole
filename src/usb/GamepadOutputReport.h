/*
 * GamepadOutputReport.h
 *
 *  Created on: Nov 15, 2009
 *      Author: fgervais
 */

#ifndef GAMEPADOUTPUTREPORT_H_
#define GAMEPADOUTPUTREPORT_H_

#include "OutputReport.h"

#include <stdint.h>

class GamepadOutputReport: public OutputReport {
public:
	uint8_t command;

	GamepadOutputReport(uint8_t ledPattern);
	virtual ~GamepadOutputReport();

	uint8_t* toArray();
};

#endif /* GAMEPADOUTPUTREPORT_H_ */
