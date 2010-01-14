/*
 * GamepadOutputReport.cpp
 *
 *  Created on: Nov 15, 2009
 *      Author: fgervais
 */

#include "GamepadOutputReport.h"

GamepadOutputReport::GamepadOutputReport(uint8_t command) {
	/*
	 * Command reference : http://free60.org/wiki/GamePad
	 *
	 * Pattern 	 Description
	 * 0x00 	All off
	 * 0x01 	All blinking
	 * 0x02 	1 flashes, then on
	 * 0x03 	2 flashes, then on
	 * 0x04 	3 flashes, then on
	 * 0x05 	4 flashes, then on
	 * 0x06 	1 on
	 * 0x07 	2 on
	 * 0x08 	3 on
	 * 0x09 	4 on
	 * 0x0A 	Rotating (e.g. 1-2-4-3)
	 * 0x0B 	Blinking*
	 * 0x0C 	Slow blinking*
	 * 0x0D 	Alternating (e.g. 1+4-2+3), then back to previous*
	 */

	//type = 0x01;
	length = 12;
	this->command = command;
}

GamepadOutputReport::~GamepadOutputReport() {

}

uint8_t* GamepadOutputReport::toArray() {
	rawReport = new uint8_t[12];

	// This is coming from Linux kernel xpad driver
	// and some USB reverse engineering
	rawReport[0] = 0x00;
	rawReport[1] = 0x00;
	rawReport[2] = 0x08;
	rawReport[3] = 0x40 + (command % 0x0E);
	rawReport[4] = 0x00;
	rawReport[5] = 0x00;
	rawReport[6] = 0x00;
	rawReport[7] = 0x00;
	rawReport[8] = 0x00;
	rawReport[9] = 0x00;
	rawReport[10] = 0x00;
	rawReport[11] = 0x00;

	return rawReport;

}
