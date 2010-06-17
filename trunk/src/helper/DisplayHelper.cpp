/*
 * DisplayHelper.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: fgervais
 */

#include "DisplayHelper.h"
#include "LCDControllerDriver.h"
#include "font5x7.h"

DisplayHelper::DisplayHelper(LCDControllerDriver* lcd) {
	this->lcd = lcd;
}

DisplayHelper::~DisplayHelper() {

}

/**
 * Draw an image on the LCD screen.
 *
 * It is important to note that this function is unsafe in the way that it doesn't check if you
 * are writing outside the screen area. This is as part of the speed/safety tradeoff. I may add
 * a safe version of this function a little later.
 *
 * @param x Start position
 * @param y Start position
 * @param imageBase Handle of the image
 * @param imageWidth Image width
 * @param imageHeight Image Height
 */
void DisplayHelper::drawImage(uint32_t x, uint32_t y, uint32_t* imageBase, uint32_t imageWidth, uint32_t imageHeight) {
	uint32_t nrows = lcd->getHeight();
	uint32_t ncols = lcd->getWidth();
	uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();

	if(x < ncols && y < nrows) {

		for (uint32_t i=0; i<imageHeight; i++) {
			for (uint32_t j=0; j<imageWidth; j++) {
				lcd_ptr[(i+y)*ncols + (j+x)] = imageBase[i*imageWidth + j];

			}
		}

	}
}

/**
 * Print a string on the screen using black font on white background.
 *
 * @param x X Start position
 * @param y Y Start position
 * @param string String to print
 */
void DisplayHelper::drawString(uint32_t x, uint32_t y, const char* string) {
	drawString(x,y,string,0x00000000, 0x00FFFFFF);
}

/**
 * Print a string on the screen.
 *
 * It is important to note that this function is unsafe in the way that it doesn't check if you
 * are writing outside the screen area. This is as part of the speed/safety tradeoff. I may add
 * a safe version of this function a little later.
 *
 * @param x X Start position
 * @param y Y Start position
 * @param string String to print
 * @param fontColor Color of the font
 * @param bgColor Color of the background
 */
void DisplayHelper::drawString(uint32_t x, uint32_t y, const char* string, uint32_t fontColor, uint32_t bgColor) {
	uint32_t nrows = lcd->getHeight();
	uint32_t ncols = lcd->getWidth();
	uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();
	uint32_t stringIndex;
	uint32_t charPosition;
	uint32_t currentChar;

	stringIndex = 0;
	charPosition = x;

	// Are we trying to print outside the screen
	// This test is not enough to know if we will eventually print outside the screen
	if(x < ncols && y < nrows) {
		while(string[stringIndex] != 0) {
			// Avoid printing a character for which we don't have the font
			if((string[stringIndex] < 0x20) || (string[stringIndex]  > 0x7f)) {
				// If it's an unknown character, lets print a space
				currentChar = ' ';
			}
			// We just want to avoid doing this over and over in the loop
			currentChar = string[stringIndex]-' ';
			// Draw the character
			for (uint32_t i=0; i<FONTHEIGHT; i++) {
				for (uint32_t j=0; j<FONTWIDTH; j++) {
					if((font5x7[(uint8_t)(currentChar)][i] & (0x80>>j)) == 0) {
						lcd_ptr[(i+y)*ncols + (j+charPosition)] = bgColor;
					}
					else {
						lcd_ptr[(i+y)*ncols + (j+charPosition)] = fontColor;
					}

				}
			}
			// Point to the next character of the string
			stringIndex++;
			// Point to the next character position on the screen
			charPosition += FONTWIDTH;
		}
	}
}

