/*
 * HCDRequest.h
 *
 *  Created on: Jun 30, 2010
 *      Author: fgervais
 */

#ifndef HCDREQUEST_H_
#define HCDREQUEST_H_

#include <stdint.h>

class UsbDevice;
class HCDEventListener;

class HCDRequest {
public:
	HCDRequest();
	virtual ~HCDRequest();

	UsbDevice* device;
	uint8_t interfaceIndex;
	uint8_t endpointIndex;
	uint8_t* transactionBuffer;
	uint32_t transactionLength;
	HCDEventListener* listener;
	uint32_t tag;	// Optional tag may be used by the device driver
};

#endif /* HCDREQUEST_H_ */
