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

/**
 * Set an optional tag to the peripheral.
 *
 * This tag must be defined in the PeripheralTah enum.
 *
 * @param tag
 */
void Peripheral::setTag(PeripheralTag tag) {
	this->tag = tag;
}
