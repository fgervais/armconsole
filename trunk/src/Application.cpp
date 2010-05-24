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
#include "ff.h"
#include "Bitmap.h"
#include "Wave.h"
#include "AudioHelper.h"
#include "DAC.h"
#include "Engine.h"

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

	// Set background
	lcd->setBackground(0x00FFFFFF);

	DisplayHelper* displayHelper = new DisplayHelper(lcd);

	/*
	 * SD Card and file system section
	 *
	 * This is all C for now
	 */
	Debug::writeLine("Init Timer");
	TIMER0->TCR = 0;				/* Disable Timer */
	TIMER0->MR0 = 720000 - 1;		/* 72M / 100 = 720000 */
	TIMER0->MCR = 0x3;			/* Clear TC and Interrupt on MR0 match */
	TIMER0->TCR = 1;				/* Enable Timer */
	VICIntSelect &= ~(1 << TIMER0_INT); // IRQ Category (Not FIQ)
	VICIntEnable |= (1 << TIMER0_INT);

	IntEnable();

	FRESULT f_err_code;
	FATFS FATFS_Obj[1];

	f_err_code = f_mount(0, &FATFS_Obj[0]);

	if(f_err_code == 0) {
		Debug::writeLine("SD card mounted");
	}
	else {
		Debug::writeLine("Failed to mount SD card");
	}

	//------ Engine Test

	Engine* engine = new Engine();
	engine->start();

	//------------------




	Bitmap* bitmap = new Bitmap("0:mmx1.bmp");
	bitmap->load();
	if(bitmap->getData() != 0) {
		displayHelper->drawImage(100,50,bitmap->getData(),bitmap->getInfoHeader()->width,bitmap->getInfoHeader()->height);
	}

	AudioHelper* audioHelper = new AudioHelper(LPC2478::getDAC());
	Wave* wave = new Wave("0:intro.wav");
	wave->load();

	//LPC2478::getDAC()->setValue(32768);
	//DAC0->DACR = 32768;
	//while(1);

	if(wave->isLoaded()) {
		Debug::writeLine("Playing wave file");
		audioHelper->play(wave);
	}
	while(1);

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
	}*/

	led->setLow();

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
