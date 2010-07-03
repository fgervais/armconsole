/*
 * GamepadInputReport.cpp
 *
 *  Created on: Nov 18, 2009
 *      Author: fgervais
 */

#include "GamepadInputReport.h"

GamepadInputReport::GamepadInputReport() {
	messageType		= 0;
	packetSize		= 0;
	buttons[0]		= 0;
	buttons[1]		= 0;
	leftTrigger		= 0;
	rightTrigger	= 0;
	LeftStickX		= 0;
	LeftStickY		= 0;
	rightStickX		= 0;
	rightStickY		= 0;

	dPadUp			= 0;
	dPadDown		= 0;
	dPadLeft		= 0;
	dPadRight		= 0;
	start			= 0;
	back			= 0;
	leftStickPress	= 0;
	rightStickPress	= 0;

	lb				= 0;
	rb				= 0;
	xboxButton		= 0;
	// Unused		= 0;
	a				= 0;
	b				= 0;
	x				= 0;
	y				= 0;
}

GamepadInputReport::GamepadInputReport(uint8_t* rawReport) {

}

GamepadInputReport::~GamepadInputReport() {

}

void GamepadInputReport::fill(uint8_t* rawReport) {
	messageType		= rawReport[0];
	packetSize		= rawReport[1];
	buttons[0]		= rawReport[2];
	buttons[1]		= rawReport[3];
	leftTrigger		= rawReport[4];
	rightTrigger	= rawReport[5];
	LeftStickX		= (rawReport[7] << 8) | rawReport[6];
	LeftStickY		= (rawReport[9] << 8) | rawReport[8];
	rightStickX		= (rawReport[11] << 8) | rawReport[10];
	rightStickY		= (rawReport[13] << 8) | rawReport[12];

	dPadUp			= (rawReport[2] & 0x01);
	dPadDown		= ((rawReport[2] & 0x02) >> 1);
	dPadLeft		= ((rawReport[2] & 0x04) >> 2);
	dPadRight		= ((rawReport[2] & 0x08) >> 3);
	start			= ((rawReport[2] & 0x10) >> 4);
	back			= ((rawReport[2] & 0x20) >> 5);
	leftStickPress	= ((rawReport[2] & 0x40) >> 6);
	rightStickPress	= ((rawReport[2] & 0x80) >> 7);

	lb				= (rawReport[3] & 0x01);
	rb				= ((rawReport[3] & 0x02) >> 1);
	xboxButton		= ((rawReport[3] & 0x04) >> 2);
	// Unused		= ((rawReport[3] & 0x08) >> 3);
	a				= ((rawReport[3] & 0x10) >> 4);
	b				= ((rawReport[3] & 0x20) >> 5);
	x				= ((rawReport[3] & 0x40) >> 6);
	y				= ((rawReport[3] & 0x80) >> 7);
}
