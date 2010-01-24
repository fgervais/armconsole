/*
 * HostControllerDriver.cpp
 *
 *  Created on: Jan 22, 2010
 *      Author: fgervais
 */

#include "HostControllerDriver.h"

HostControllerDriver::HostControllerDriver() {


}

HostControllerDriver::~HostControllerDriver() {

}

void HostControllerDriver::init() {
	hcca = (Hcca *)0x7FD00000;
}
