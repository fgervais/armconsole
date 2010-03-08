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

void Debug::writeLine(char* string) {
	if(console == 0) {
		console = new Console(LPC2478::getLCD());
	}
	console->writeLine(string);
}