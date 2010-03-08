/*
 * Console.cpp
 *
 *  Created on: Mar 6, 2010
 *      Author: fgervais
 */

#include "Console.h"
#include "LCDControllerDriver.h"
#include "DisplayHelper.h"
#include "font5x7.h"
#include "LPC2478.h"

Console::Console(LCDControllerDriver* lcd) {
	this->lcd = lcd;
	displayHelper = new DisplayHelper(lcd);

	nbLine = lcd->getHeight()/FONTHEIGHT;
	nbCol = lcd->getWidth()/FONTWIDTH;

	currentLine = 0;
	bottomReached = 0;

	// Allocate enough memory to fill all the screen with text
	// This is kind of RAM hungry. Pay attention.
	text = new char*[nbLine];
	for(uint32_t i=0; i<nbLine; i++) {
		text[i] = new char[nbCol];
	}
}

Console::~Console() {
	/* Free memory */
	for(uint8_t i=0; i<nbLine; i++) {
		delete[] text[i];
	}
	delete[] text;
}

/**
 * Write a line in the console.
 *
 * @param string Line to be written
 */
void Console::writeLine(char* string) {
	uint32_t j;

	if(bottomReached) {
		scrollDown();
	}

	// Fill the current line with the string
	for(j=0; j<nbCol && string[j] != 0; j++) {
		text[currentLine][j] = string[j];
	}
	// Fill the rest of the line with white space
	for(; j<nbCol; j++) {
		text[currentLine][j] = ' ';
	}
	// Null terminate the string (line)
	text[currentLine][j-1] = 0;

	// Display the current line on the screen
	displayHelper->drawString(0,currentLine*FONTHEIGHT,text[currentLine]);

	if(currentLine < (nbLine-1)) {
		currentLine++;
	}
	else if(!bottomReached) {
		bottomReached = 1;
	}
}

/**
 * Used to scroll the text of the console down.
 */
void Console::scrollDown() {
	for(uint32_t i=0; i<currentLine; i++) {
		for(uint32_t j=0; j<nbCol; j++) {
			// Copy the lower line string to the upper line.
			text[i][j] = text[i+1][j];
		}
	}
	refresh();
}

/**
 * Display text on the screen.
 */
void Console::refresh() {
	for(uint32_t i=0; i<=currentLine; i++) {
		displayHelper->drawString(0,i*FONTHEIGHT,text[i]);
	}
}
