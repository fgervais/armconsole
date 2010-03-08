/*
 * Console.h
 *
 *  Created on: Mar 6, 2010
 *      Author: fgervais
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>

class LCDControllerDriver;
class DisplayHelper;

/**
 * This class is used to use the screen in a console
 * like fashion.
 */
class Console {
public:
	Console(LCDControllerDriver* lcd);
	virtual ~Console();

	void writeLine(char* string);

private:
	LCDControllerDriver* lcd;
	DisplayHelper* displayHelper;
	char** text;
	uint32_t nbLine;
	uint32_t nbCol;
	uint32_t currentLine;

	// Flags
	uint8_t bottomReached;

	void writeLine();
	void scrollDown();
	void refresh();
};

#endif /* CONSOLE_H_ */
