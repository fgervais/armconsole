/*
 * Peripheral.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <stdint.h>

class Peripheral {
public:
	enum PeripheralTag { None =0, /* Add tags here*/ };

	Peripheral();
	Peripheral(uint8_t id);
	virtual ~Peripheral();

	uint8_t getId() { return id; }
	void setTag(PeripheralTag tag);
	PeripheralTag getTag() { return tag; }

private:
	uint8_t id;
	PeripheralTag tag;
};

#endif /* PERIPHERAL_H_ */
