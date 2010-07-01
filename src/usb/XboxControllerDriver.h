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

	XboxControllerDriver(UsbDevice* device);
	virtual ~XboxControllerDriver();

	// HCDEventListener implementation
	virtual void transferCompleted(HCDRequest*);

	void configure();
	void setLedState(LedState, uint8_t controllerNumber);
	void setRumbleState(uint8_t smallSpeed, uint8_t largeSpeed, uint8_t controllerNumber);
	void getStatus(uint8_t controllerNumber);

private:
	static const uint8_t NON_PERIODIC_REQUEST = 0;
	static const uint8_t PERIODIC_REQUEST = 1;

	UsbDevice* device;

	HCDRequest ledStateRequest;
	HCDRequest rumbleStateRequest;
	HCDRequest statusRequest;

	GamepadInputReport gamepadStatus;
};

#endif /* XBOXCONTROLLERDRIVER_H_ */
