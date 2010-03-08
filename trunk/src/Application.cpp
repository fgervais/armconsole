/*
 * LPC2478.h
 *
 *  Created on: Dec 31, 2009
 *      Author: fgervais
 */

#include "GpioConfiguration.h"
#include "GpioPinConfiguration.h"
#include "Gpio.h"
#include "GpioPin.h"
#include "LPC2478.h"
#include "HostControllerDriver.h"
#include "LCDControllerDriver.h"
#include "LCDConfiguration.h"
#include "math.h"
#include "DisplayHelper.h"
#include "Debug.h"

#include "irq.h"
#include "swi.h"
//#include "megaman.h"
//#include "megaman_running.h"

#include <stdint.h>

int main() {
	Gpio *gpio1 = LPC2478::getGpio1();

	// Set default port behavior
	GpioConfiguration portConfig(32, Gpio::INPUT);
	gpio1->configure(portConfig);

	// Configure blinking led
	GpioPinConfiguration *ledPinConfig = new GpioPinConfiguration();
	ledPinConfig->pin = Gpio::OUTPUT;
	gpio1->getPin(12)->configure(*ledPinConfig);

	GpioPin *led = gpio1->getPin(12);

	// USB debug section
	//HostControllerDriver* hcd = LPC2478::getHCD();
	//IntEnable();

	LCDConfiguration lcdConfig;
	lcdConfig.bufferBaseAddress = 0xA0000000;
	lcdConfig.height = 272;
	lcdConfig.width = 480;
	LCDControllerDriver* lcd = LPC2478::getLCD();
	lcd->configure(lcdConfig);

	//DisplayHelper* displayHelper = new DisplayHelper(lcd);
	// Display megaman screenshot
	//displayHelper->drawImage(112,24,(uint32_t*)megaman,256,224);

	// Set white background
	lcd->setBackground(0x00FFFFFF);

	//displayHelper->drawString(0, 50,"String");
	for(uint32_t i=0; i<17; i++) {
		Debug::writeLine("Debug 1");
		Debug::writeLine("Debug 2 longer line longer line longer line longer line longer line longer line longer line longer line longer line longer line");
	}
	Debug::writeLine("Debug 3");
	Debug::writeLine("Debug 4");
	Debug::writeLine("Debug 3");
	Debug::writeLine("Debug 4");
	Debug::writeLine("Debug 3");
	Debug::writeLine("Debug 4");
	//Debug::writeLine("Debug 4");

	// Display megaman gif
	/*uint32_t* lcd_ptr = (uint32_t*)lcd->getBufferBase();
	uint32_t bufferLenght = 480*272;
	displayHelper->drawImage(50,50,(uint32_t*)&megaman_running[0][0],35,35);
	while(1) {
		for(uint8_t i=1;i<11; i++) {
			displayHelper->drawImage(0,0,(uint32_t*)&megaman_running[i][0],35,35);
			//LPC2478::delay(100000);
			for (uint32_t i=16800; i<bufferLenght; i++) {
				*(lcd_ptr++) = 0x00FFFFFF;
			}
		}
	}

	led->setLow();

	while(1);*/

	// Test sdram
#define SDRAM_BASE_ADDR *(volatile unsigned int*)0xA0000000

	/*unsigned int i;
	for (i = 0; i < 0x2000000; i+=sizeof(unsigned int))
	{
	*(unsigned int *)((unsigned int )&SDRAM_BASE_ADDR+i) = i;
	}

	for (i = 0; i < 0x2000000; i+=sizeof(unsigned int ))
	{
	if (*(unsigned int *)((unsigned int )&SDRAM_BASE_ADDR+i) != i)
	{
	  led->setHigh();
	  while(1);
	}
	}*/

	//while(1);
	//LPC2478::delay(1000000);

	// Blink led
	while(1) {
		led->setHigh();	// On
		//for(uint32_t i=0; i<100000; i++);
		LPC2478::delay(1000000);
		led->setLow();	// Off
		//for(uint32_t i=0; i<100000; i++);
		LPC2478::delay(1000000);
	}

}
