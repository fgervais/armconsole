/*
 * SetConfiguration.h
 *
 *  Created on: Nov 14, 2009
 *      Author: fgervais
 */

#ifndef SETCONFIGURATION_H_
#define SETCONFIGURATION_H_

#include "ControlRequest.h"

class SetConfiguration: public ControlRequest {
public:
	SetConfiguration(uint8_t configuration);
	virtual ~SetConfiguration();
};

#endif /* SETCONFIGURATION_H_ */
