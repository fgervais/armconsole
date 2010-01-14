/*
 * KeyboardReport.h
 *
 *  Created on: Nov 5, 2009
 *      Author: fgervais
 */

#ifndef KEYBOARDREPORT_H_
#define KEYBOARDREPORT_H_

#include "InputReport.h"

#include <stdint.h>

class KeyboardReport: public InputReport {
public:
	/*
	 * The bitmap of Modifier byte
	 * bit
	 * 0   LEFT CTRL
	 * 1   LEFT SHIFT
	 * 2   LEFT ALT
	 * 3   LEFT GUI
	 * 4   RIGHT CTRL
	 * 5   RIGHT SHIFT
	 * 6   RIGHT ALT
	 * 7   RIGHT GUI
	 */
	uint8_t modifier;
	/*
	 * Keycodes can be found in the
	 * Universal Serial Bus HID Usage Tables
	 * www.usb.org/developers/devclass_docs/Hut1_12.pdf
	 */
	uint8_t keycode[6];

	KeyboardReport(uint8_t* rawReport);
	virtual ~KeyboardReport();
};

#endif /* KEYBOARDREPORT_H_ */
