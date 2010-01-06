/*
 * Peripheral.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "Peripheral.h"

#include <stdint.h>

Peripheral::Peripheral() {
	this->id = 0;
	this->tag = None;
}

Peripheral::Peripheral(uint8_t id) {
	this->id = id;
	this->tag = None;
}

Peripheral::~Peripheral() {

}

void Peripheral::setTag(PeripheralTag tag) {
	this->tag = tag;
}
