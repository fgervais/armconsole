/*
 * SetAddress.h
 *
 *  Created on: Oct 28, 2009
 *      Author: fgervais
 */

#ifndef SETADDRESS_H_
#define SETADDRESS_H_

#include "ControlRequest.h"

class SetAddress: public ControlRequest {
public:
	SetAddress(uint16_t address);
	virtual ~SetAddress();
};

#endif /* SETADDRESS_H_ */
