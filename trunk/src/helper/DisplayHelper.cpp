/*
 * DisplayHelper.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#include "DisplayHelper.h"
#include "LCDControllerDriver.h"

DisplayHelper::DisplayHelper(LCDControllerDriver* lcd) {
	this->lcd = lcd;
}

DisplayHelper::~DisplayHelper() {

}
