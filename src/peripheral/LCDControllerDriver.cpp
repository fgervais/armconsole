/*
 * LCDControllerDriver.cpp
 *
 *  Created on: Feb 21, 2010
 *      Author: fgervais
 */

#include "LCDControllerDriver.h"
#include "LCDConfiguration.h"
#include "LPC2478.h"

LCDControllerDriver::LCDControllerDriver(LCD_Typedef* lcdRegisters) {
	this->lcdRegisters = lcdRegisters;

	lcdState = Down;
}

LCDControllerDriver::~LCDControllerDriver() {

}

/**
 * LCD controller power up sequence.
 *
 * According to the datasheet, this sequence must be
 * strictly followed.
 *
 * @see datasheet for more information
 */
void LCDControllerDriver::powerUp() {
	lcdRegisters->LCD_CTRL |= 1;			// LCD enable
	LPC2478::delay(50000);					// 50 ms
	lcdRegisters->LCD_CTRL |= (1 << 11);	// LCD power enable
	lcdState = Up;
}

/**
 * LCD controller power down sequence.
 *
 * According to the datasheet, this sequence must be
 * strictly followed.
 *
 * @see datasheet for more information
 */
void LCDControllerDriver::powerDown() {
	lcdRegisters->LCD_CTRL &= ~(1 << 11);	// LCD power enable
	LPC2478::delay(50000);					// 50 ms
	lcdRegisters->LCD_CTRL &= ~1;			// LCD enable
	lcdState = Down;
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
	uint32_t regVal;

	// Save the information
	this->bufferBase = config.bufferBaseAddress;
	this->lcdHeight = config.height;
	this->lcdWidth = config.width;

	/*
	 * IMPORTANT NOTICE
	 *
	 * The LCD controller clock divider is set in lowlevel/target.c
	 * This is where all the clocking work is done.
	 */

	/*
	 * If don't know if this is required, but I prefer to have the
	 * LCD powered down when playing with the controller timings.
	 *
	 * Note that there is not mention in the datasheet that this
	 * should be done.
	 */
	if(lcdState == Up) {
		powerDown();
	}

	// Set pixel per line
	regVal = lcdRegisters->LCD_TIMH;
	regVal &= 0xFFFFFF03;
	regVal |= (((config.width >> 4)-1) & 0x3F) << 2;
	lcdRegisters->LCD_TIMH = regVal;

	// Set line per panel
	regVal = lcdRegisters->LCD_TIMV;
	regVal &= 0xFFFFFC00;
	regVal |= ((config.height-1) & 0x3FF);
	lcdRegisters->LCD_TIMV = regVal;

	regVal = 0;
	regVal |= (1 << 11);	//invert vertical sync
	regVal |= (1 << 12);	//invert horizontal sync
	regVal |= ((config.width-1) << 16);	//clock per line
	regVal |= (1 << 26);	//bypass pixel clock divider
	lcdRegisters->LCD_POL = regVal;

	lcdRegisters->LCD_LE = 0;	//disable LE pin

	lcdRegisters->LCD_UPBASE = config.bufferBaseAddress;

	regVal = 0;
	regVal |= (5 << 1); //24 bpp
	regVal |= (1 << 5); //TFT type
	//regVal |= (0 << 8); //RGB format
	//regVal |= (0 << 12); //vertical compare interrupt generated at start of vertical sync
	//regVal |= (0 << 16); //DMA FIFO watermark level : 4 or more
	lcdRegisters->LCD_CTRL = regVal;

	lcdRegisters->LCD_INTMSK = 0;

	/*
	 * The BlueScreen SUN team change de default round-robin
	 * AHB1 access scheduler to a priority based one with
	 * LCD getting the highest priority. I'll try using the
	 * default stuff and we'll see how it goes.
	 */
	//AHBCFG1 = 0x12340144;
}

void LCDControllerDriver::clearScreen() {
	uint32_t* lcd_ptr = (uint32_t*)bufferBase;
	uint32_t bufferLength = lcdWidth*lcdHeight;

	for (uint32_t i=0; i<bufferLength; i++) {
		*(lcd_ptr++) = 0x00000000;
	}
}
