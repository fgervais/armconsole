/*
 * DMAConfiguration.h
 *
 *  Created on: Mar 13, 2010
 *      Author: fgervais
 */

#ifndef DMACONFIGURATION_H_
#define DMACONFIGURATION_H_

#include "DMAChannel.h"

class DMAConfiguration {
public:
	DMAConfiguration();
	virtual ~DMAConfiguration();

	DMACCxLLI* lli;
};

#endif /* DMACONFIGURATION_H_ */
