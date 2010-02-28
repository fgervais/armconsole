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

#include "irq.h"
#include "swi.h"
#include "megaman.h"

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

	DisplayHelper* displayHelper = new DisplayHelper(lcd);
	displayHelper->drawImage(112,24,(uint32_t*)megaman,256,224);

	led->setLow();

	while(1);

	uint32_t* lcd_ptr = (uint32_t*)0xA0000000;
	uint32_t bufferLength = 480*272;
	uint32_t r,g,b;

	/*for (uint32_t i=0; i<bufferLength; i++) {
		*(lcd_ptr++) = 0x00FFFF00 | (i & 0xFF);
	}
	while(1);*/

	/*uint32_t nrows = 272;
	uint32_t ncols = 480;
	for (uint32_t i = 0; i < nrows; ++i) {
		for (uint32_t j = 0; j < ncols; ++j) {
			//lcd_ptr[i*ncols + j] = i*j;
			//lcd_ptr[i*ncols + j] = sqrt(pow(i,2)+pow(j,2));
			//lcd_ptr[i*ncols + j] = pow(i,2)+pow(j,2);
			//lcd_ptr[i*ncols + j] = (0xff << 16) | (uint32_t)((float)0x7f * (sin((float)(j/10)) + 1.0));

		}
	}
	while(1);*/

	r=255;
	g=0;
	b=0;
	while(1) {
		for(;g<251;g+=4) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;r>8;r-=8) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;b<247;b+=8) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;g>4;g-=4) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;r<247;r+=8) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;b>8;b-=8) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
	}

	r=255;
	g=0;
	b=0;
	while(1) {
		for(;g<255;g+=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;r>0;r-=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;b<255;b+=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;g>0;g-=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;r<255;r+=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
		for(;b>0;b-=1) {
			lcd_ptr = (uint32_t*)0xA0000000;
			for (uint32_t i=0; i<bufferLength; i++) {
				*(lcd_ptr++) = (b << 16) | (g << 8) | r;
			}
		}
	}


	while(1) {
		for(r=0; r<255; r+=16) {
			for(g=0; g<255; g+=1) {
				for(b=0; b<255; b+=1) {
					lcd_ptr = (uint32_t*)0xA0000000;
					for (uint32_t i=0; i<bufferLength; i++) {
						*(lcd_ptr++) = (b << 16) | (g << 8) | r;
						//LPC2478::delay(10);
					}

				}

			}
		}
	}


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
