/*
 * ControlPacket.h
 *
 *  Created on: Oct 27, 2009
 *      Author: fgervais
 */

#ifndef CONTROLREQUEST_H_
#define CONTROLREQUEST_H_

#include <stdint.h>

class ControlRequest {
public:
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;

	uint8_t* toArray();

	ControlRequest();
	virtual ~ControlRequest();

private:
	uint8_t* rawPacket;
};

#endif /* CONTROLREQUEST_H_ */
