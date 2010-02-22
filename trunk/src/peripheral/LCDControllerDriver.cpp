/*
 * LCDControllerDriver.cpp
 *
 *  Created on: Feb 21, 2010
 *      Author: fgervais
 */

#include "LCDControllerDriver.h"
#include "LCDConfiguration.h"

LCDControllerDriver::LCDControllerDriver(LCD_Typedef* lcdRegisters) {
	this->lcdRegisters = lcdRegisters;

	init();
}

LCDControllerDriver::~LCDControllerDriver() {

}

void LCDControllerDriver::init() {

}

/**
 * Configure the LCD controller.
 *
 * A lot of this is coming from the Catalog Demo (POS Demo)
 * by Bluescreen SUN (ThaiEasyElec)
 *
 * @param config LCD controller configuration
 */
void LCDControllerDriver::configure(LCDConfiguration config) {
	/*LCD_CTRL = 0;
	LCD_CFG = LCD_CLK_DIV-1;
	buf = 0;
	buf |= (((LCD_WIDTH >> 4)-1) & 0x3F) << 2;//pixel per line
	buf |= 40 << 8; //horizontal sync pulse width
	buf |= 1 << 16; //horizontal front porch
	buf |= 1 << 24; //horizontal back porch
	LCD_TIMH = buf;
	buf = 0;
	buf |= ((LCD_HEIGHT-1) & 0x3FF);//line per panel
	buf |= 9 << 10; //vertical sync pulse width
	buf |= 1 << 16; //vertical front porch
	buf |= 1 << 24; //vertical back porch
	LCD_TIMV = buf;
	buf = 0;
	buf |= 1 << 11; //invert vertical sync
	buf |= 1 << 12; //invert horizontal sync
	//buf |= 1 << 13; //invert panel clock
	buf |= (LCD_WIDTH-1) << 16; //clock per line
	buf |= 1 << 26; //bypass pixel clock divider
	LCD_POL = buf;
	LCD_LE = 0; //disable LE pin
	LCD_UPBASE = LCD_BUFFER_BASE;
	//LCD_LPBASE = LCD_BUFFER_BASE;
	buf = 0;
	buf |= (5 << 1); //24 bpp
	buf |= (1 << 5); //TFT type
	//buf |= (0 << 8); //RGB format
	//buf |= (0 << 12); //vertical compare interrupt generated at start of vertical sync
	//buf |= (0 << 16); //DMA FIFO watermark level : 4 or more
	LCD_CTRL = buf;
	LCD_INTMSK = 0;

	LCD_CTRL |= 1; //LCD enable
	delay_1ms(50);
	LCD_CTRL |= 1 << 11; //LCD power enable
	Setb(LCD_ADJ_PRTS,LCD_ADJ_PIN);*/

	/*
	 * IMPORTANT NOTICE
	 *
	 * The LCD controller clock divider is set in lowlevel/target.c
	 * This is where all the clocking work is done.
	 */

}
