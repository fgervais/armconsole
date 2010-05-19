/*
 * Debug.cpp
 *
 *  Created on: Mar 6, 2010
 *      Author: fgervais
 */

#include "Debug.h"
#include "LPC2478.h"
#include "Console.h"
#include "LCDControllerDriver.h"

/* Static initialization. Required to make the compiler happy */
Console* Debug::console = 0;

Debug::Debug() {
	console = new Console(LPC2478::getLCD());
}

Debug::~Debug() {

}

void Debug::writeLine(const char* string) {
	//TODO: This is a hack since it should be called from the constructor
	if(console == 0) {
		console = new Console(LPC2478::getLCD());
	}
	console->writeLine(string);
}
