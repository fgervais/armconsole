/*
 * GamepadInputReport.h
 *
 *  Created on: Nov 18, 2009
 *      Author: fgervais
 */

#ifndef GAMEPADINPUTREPORT_H_
#define GAMEPADINPUTREPORT_H_

#include "InputReport.h"

#include <stdint.h>

class GamepadInputReport: public InputReport {
public:
	// Reference : http://free60.org/wiki/GamePad

	/*
	 * 0x01 : LED status message
	 * 0x00 : normal input report message.
	 */
	uint8_t messageType;
	/*
	 * Should be 20 bytes (0x14)
	 */
	uint8_t packetSize;
	/*
	 * bit (buttons[0])
	 * 0 D-Pad up
	 * 1 D-Pad down
	 * 2 D-Pad left
	 * 3 D-pad right
	 * 4 Start button
	 * 5 Back button
	 * 6 Left stick press
	 * 7 Right stick press
	 *
	 * bit (buttons[1])
	 * 0 Button LB
	 * 1 Button RB
	 * 2 Xbox logo button
	 * 3 Unused
	 * 4 Button A
	 * 5 Button B
	 * 6 Button X
	 * 7 Button Y
	 */
	uint8_t buttons[2];
	uint8_t leftTrigger;
	uint8_t rightTrigger;
	int16_t LeftStickX;
	int16_t LeftStickY;
	int16_t rightStickX;
	int16_t rightStickY;

	// All 8 buttons mapped to a byte variable
	uint8_t dPadUp;
	uint8_t dPadDown;
	uint8_t dPadLeft;
	uint8_t dPadRight;
	uint8_t start;
	uint8_t back;
	uint8_t leftStickPress;
	uint8_t rightStickPress;

	uint8_t lb;
	uint8_t rb;
	uint8_t xboxButton;
	uint8_t a;
	uint8_t b;
	uint8_t x;
	uint8_t y;

	GamepadInputReport(uint8_t* rawReport);
	GamepadInputReport();
	virtual ~GamepadInputReport();
};

#endif /* GAMEPADINPUTREPORT_H_ */
