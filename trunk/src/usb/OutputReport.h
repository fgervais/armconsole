/*
 * OutputReport.h
 *
 *  Created on: Nov 15, 2009
 *      Author: fgervais
 */

#ifndef OUTPUTREPORT_H_
#define OUTPUTREPORT_H_

#include <stdint.h>

class OutputReport {
public:
	uint8_t type;
	uint8_t length;
	uint8_t* rawReport;

	OutputReport();
	virtual ~OutputReport();

	virtual uint8_t* toArray() =0;
};

#endif /* OUTPUTREPORT_H_ */
