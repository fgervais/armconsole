/*
 * OutputReport.cpp
 *
 *  Created on: Nov 15, 2009
 *      Author: fgervais
 */

#include "OutputReport.h"

OutputReport::OutputReport() {

}

OutputReport::~OutputReport() {
	// Free memory
	delete rawReport;
}
