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
void DisplayHelper::drawString(uint32_t x, uint32_t y, char* string) {
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
void DisplayHelper::drawString(uint32_t x, uint32_t y, char* string, uint32_t fontColor, uint32_t bgColor) {
#define FONTWIDTH	6
#define FONTHEIGHT	8

	uint32_t nrows = lcd->getHeight();
	uint32_t ncols = lcd->getWidth();
	uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();
	uint32_t stringIndex;
	uint32_t charPosition;

	stringIndex = 0;
	charPosition = x;

	// Are we trying to print outside the screen
	if(x < ncols && y < nrows) {
		while(string[stringIndex] != 0) {
			for (uint32_t i=0; i<FONTHEIGHT; i++) {
				for (uint32_t j=0; j<FONTWIDTH; j++) {
					if((font5x7[(uint8_t)(string[stringIndex]-' ')][i] & (0x80>>j)) == 0) {
						lcd_ptr[(i+y)*ncols + (j+charPosition)] = bgColor;
					}
					else {
						lcd_ptr[(i+y)*ncols + (j+charPosition)] = fontColor;
					}

				}
			}
			stringIndex++;
			charPosition += FONTWIDTH;
		}
	}
}

void DisplayHelper::drawChar(uint32_t x, uint32_t y, char ch, uint32_t fontColor, uint32_t bgColor) {
#define FONTWIDTH	6
#define FONTHEIGHT	8

	uint32_t nrows = lcd->getHeight();
	uint32_t ncols = lcd->getWidth();
	uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();

	if( (ch < 0x20) || (ch > 0x7f) ) {
		ch = 0x20;		/* unknown character will be set to blank */
	}

	if(x < ncols && y < nrows) {
		for (uint32_t i=0; i<FONTHEIGHT; i++) {
			for (uint32_t j=0; j<FONTWIDTH; j++) {
				if((font5x7[(uint8_t)(ch-' ')][i] & (0x80>>j)) == 0) {
					lcd_ptr[(i+y)*ncols + (j)] = bgColor;
				}
				else {
					lcd_ptr[(i+y)*ncols + (j)] = fontColor;
				}

			}
		}
	}
}
