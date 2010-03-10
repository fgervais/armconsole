/*
 * DemoLevel.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef DEMOLEVEL_H_
#define DEMOLEVEL_H_

#include "Environment.h"

class DemoLevel: public Environment {
public:
	DemoLevel();
	virtual ~DemoLevel();

	virtual void build();
};

#endif /* DEMOLEVEL_H_ */
