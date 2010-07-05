/*
 * XboxControllerDriver.h
 *
 *  Created on: Jun 25, 2010
 *      Author: fgervais
 */

#ifndef XBOXCONTROLLERDRIVER_H_
#define XBOXCONTROLLERDRIVER_H_

#include <stdint.h>
#include "HCDEventListener.h"
#include "HCDRequest.h"
#include "GamepadInputReport.h"

class UsbDevice;
class MemoryPool;

/**
 * This class is the high-level driver for the
 * Xbox 360 wireless controller.
 *
 * A lot of this is based on the userspace linux
 * driver xboxdrv and on personal reverse engineering.
 *
 */
class XboxControllerDriver : HCDEventListener {
public:

	/*
	 * Warning: The order of these command
	 * in the enum is important since their
	 * position number is used as the actual
	 * command number sent to the controller.
	 */
	enum LedState {
		All_OFF =0x00,
		All_Blinking,
		Flashes_ON_1,
		Flashes_ON_2,
		Flashes_ON_3,
		Flashes_ON_4,
		ON_1,
		ON_2,
		ON_3,
		ON_4,
		Rotating_1234,
		Blinking,
		Slow_Blinking,
		Alternating_14_23
	};

	static const uint8_t CONTROLLER1 = 0;
	static const uint8_t CONTROLLER2 = 1;
	static const uint8_t CONTROLLER3 = 2;
	static const uint8_t CONTROLLER4 = 3;
	static const uint8_t NUMBER_OF_CONTROLLER = 4;

	XboxControllerDriver(UsbDevice*, MemoryPool*);
	virtual ~XboxControllerDriver();

	// HCDEventListener implementation
	virtual void transferCompleted(HCDRequest*);

	void query(uint8_t controllerNumber);
	void setLedState(LedState, uint8_t controllerNumber);
	void setRumbleState(uint8_t smallSpeed, uint8_t largeSpeed, uint8_t controllerNumber);
	GamepadInputReport* getStatus(uint8_t controllerNumber);

private:
	// Used to tag the HCDRequest so we recognize them
	// in the transferCompleted() function.
	static const uint8_t QUERY_TAG			= 1;
	static const uint8_t LED_STATE_TAG		= 2;
	static const uint8_t RUMBLE_STATE_TAG 	= 3;

	static const uint8_t NON_PERIODIC_REQUEST 	= 0;
	static const uint8_t PERIODIC_REQUEST 		= 1;

	UsbDevice* device;
	MemoryPool* memoryPool;

	// USB requests
	HCDRequest ledStateRequest[NUMBER_OF_CONTROLLER];
	HCDRequest rumbleStateRequest[NUMBER_OF_CONTROLLER];
	HCDRequest statusRequest[NUMBER_OF_CONTROLLER];

	GamepadInputReport gamepadStatus[NUMBER_OF_CONTROLLER];

	// USB memory pointers
	uint8_t* queryBuffer[NUMBER_OF_CONTROLLER];
	uint8_t* ledStateBuffer[NUMBER_OF_CONTROLLER];
	uint8_t* rumbleStateBuffer[NUMBER_OF_CONTROLLER];
};

#endif /* XBOXCONTROLLERDRIVER_H_ */
