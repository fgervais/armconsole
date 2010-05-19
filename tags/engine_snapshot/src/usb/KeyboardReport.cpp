/*
 * KeyboardReport.cpp
 *
 *  Created on: Nov 5, 2009
 *      Author: fgervais
 */

#include "KeyboardReport.h"

KeyboardReport::KeyboardReport(uint8_t* rawReport) {
	modifier = rawReport[0];
	keycode[0] = rawReport[2];
	keycode[1] = rawReport[3];
	keycode[2] = rawReport[4];
	keycode[3] = rawReport[5];
	keycode[4] = rawReport[6];
	keycode[5] = rawReport[7];
}

KeyboardReport::~KeyboardReport() {

}
