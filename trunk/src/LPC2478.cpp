/*
 * LPC2478.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "LPC2478.h"
#include "Gpio.h"
#include "HostControllerDriver.h"
#include "LCDControllerDriver.h"
#include "BlueScreenSunLCD.h"
#include "DAC.h"
#include "DMAChannel.h"
#include "Timer.h"
#include "irq.h"

/* Static initialization. Required to make the compiler happy */
Gpio* LPC2478::gpio0 = 0;
Gpio* LPC2478::gpio1 = 0;
Gpio* LPC2478::gpio2 = 0;
Gpio* LPC2478::gpio3 = 0;
HostControllerDriver* LPC2478::hcd = 0;
LCDControllerDriver* LPC2478::lcd = 0;
DAC* LPC2478::dac = 0;
DMAChannel* LPC2478::dma0 = 0;
DMAChannel* LPC2478::dma1 = 0;
Timer* LPC2478::timer0 = 0;
Timer* LPC2478::timer1 = 0;
Timer* LPC2478::timer2 = 0;
Timer* LPC2478::timer3 = 0;

LPC2478::LPC2478() {

}

LPC2478::~LPC2478() {

}

/**
 * Used to access General Purpose IO port 0.
 *
 * @return Reference to IO port 0
 */
Gpio* LPC2478::getGpio0() {
	if(gpio0 == 0) {
		// Enable High speed GPIO on ports 0 and 1
		SCS |= 1;

		gpio0 = new Gpio(FIO0, 32, 0);
	}
	return gpio0;
}

/**
 * Used to access General Purpose IO port 1.
 *
 * @return Reference to IO port 1
 */
Gpio* LPC2478::getGpio1() {
	if(gpio1 == 0) {
		// Enable High speed GPIO on ports 0 and 1
		SCS |= 1;

		gpio1 = new Gpio(FIO1, 32, 1);
	}
	return gpio1;
}

/**
 * Used to access General Purpose IO port 2.
 *
 * @return Reference to IO port 2
 */
Gpio* LPC2478::getGpio2() {
	if(gpio2 == 0) {
		gpio2 = new Gpio(FIO2, 32, 2);
	}
	return gpio2;
}

/**
 * Used to access General Purpose IO port 3.
 *
 * @return Reference to IO port 3
 */
Gpio* LPC2478::getGpio3() {
	if(gpio3 == 0) {
		gpio3 = new Gpio(FIO3, 32, 3);
	}
	return gpio3;
}

/**
 * Used to access USB Host Controller Driver.
 *
 * @return Reference to the Host Controller Driver
 */
HostControllerDriver* LPC2478::getHCD() {
	if(hcd == 0) {
		uint32_t  pinsel;
		uint32_t  pinmode;

		PCONP |= (1 << 31);
		// Disable the USB interrupt source
		VICIntEnClr    = (1 << USB_INT);
		// Enable USB host clock
		OTG_CLK_CTRL   = 0x09;
		// Wait until USB host clock is stable
		while ((OTG_CLK_STAT & 0x09) == 0);

		// The USB device controller signals are mapped to the U2 port
		USBPortSel &= ~0x03;
		USBPortSel |= 0x01;

		/* Configure USB pins */

		/*
		 * P0[12] = USB_PPWR2          01
		 * P0[13] = USB_UP_LED2        01
		 * P0[14] = USB_CONNECT2       10
		 */
		pinsel = PINSEL0;
		pinsel &= 0xC0FFFFFF;
		pinsel |= 0x15000000;
		PINSEL0 = pinsel;

		pinmode = PINMODE0;
		pinmode &= 0xCFFFFFFF;
		pinmode |= 0x30000000;
		PINMODE0 = pinmode;

		/*
		 * P0[29] = USB_D+1
		 * P0[30] = USB_D-1
		 * P0[31] = USB_D+2
		 */
		pinsel = PINSEL1;
		pinsel &= 0x03FFFFFF;
		pinsel |= 0x54000000;
		PINSEL1 = pinsel;

		/*
		 * P1[18] = USB_UP_LED1        01
		 * P1[19] = USB_PPWR1          10
		 * P1[22] = USB_PWRD1          10
		 * P1[27] = USB_OVRCR1         10
		 * P1[30] = USB_VBUS           10
		 * P1[31] = USB_OVRCR2         01
		 */
		pinsel = PINSEL3;
		pinsel &= 0x0F3F3F0F;
		pinsel |= 0x60802090;
		PINSEL3 = pinsel;

		hcd = new HostControllerDriver(OHCI);
	}
	return hcd;
}

LCDControllerDriver* LPC2478::getLCD() {
	if(lcd == 0) {
		uint32_t  pinsel;

		PCONP |= (1 << 20); //power on LCD controller

		// Time delay?

		// Enable LCD in 24 bits mode
		PINSEL11 = 0x0000000F;

		/*
		 * P0[4] = LCDVD[0]		01
		 * P0[5] = LCDVD[1]		01
		 * P0[6] = LCDVD[8]		01
		 * P0[7] = LCDVD[9]		01
		 * P0[8] = LCDVD[16]	01
		 * P0[9] = LCDVD[17]	01
		 */
		pinsel = PINSEL0;
		pinsel &= 0xFFF000FF;
		pinsel |= 0x00055500;
		PINSEL0 = pinsel;

		/*
		 * P1[20] = LCDVD[6]/LCDVD[10]		01
		 * P1[21] = LCDVD[7]/LCDVD[11]		01
		 * P1[22] = LCDVD[8]/LCDVD[12]		01
		 * P1[23] = LCDVD[9]/LCDVD[13]		01
		 * P1[24] = LCDVD[10]/LCDVD[14]		01
		 * P1[25] = LCDVD[11]/LCDVD[15]		01
		 * P1[26] = LCDVD[12]/LCDVD[20]		01
		 * P1[27] = LCDVD[13]/LCDVD[21]		01
		 * P1[28] = LCDVD[14]/LCDVD[22]		01
		 * P1[29] = LCDVD[15]/LCDVD[23]		01
		 */
		pinsel = PINSEL3;
		pinsel &= 0xF00000FF;
		pinsel |= 0x05555500;
		PINSEL3 = pinsel;

		/*
		 * P2[0] = LCDPWR				11
		 * P2[2] = LCDDCLK				11
		 * P2[3] = LCDFP				11
		 * P2[4] = LCDENAB/LCDM			11
		 * P2[5] = LCDLP				11
		 * P2[6] = LCDVD[0]/LCDVD[4]	11
		 * P2[7] = LCDVD[1]/LCDVD[5]	11
		 * P2[8] = LCDVD[2]/LCDVD[6]	11
		 * P2[9] = LCDVD[3]/LCDVD[7]	11
		 * P2[12] = LCDVD[4]/
		 *			LCDVD[3]/
		 *			LCDVD[8]/
		 *			LCDVD[18]			01
		 * P2[13] = LCDVD[5]/
		 * 			LCDVD[9]/
		 * 			LCDVD[19]			01
		 */
		pinsel = PINSEL4;
		pinsel &= 0xF0F0000C;
		pinsel |= 0x050FFFF3;
		PINSEL4 = pinsel;

		PINSEL4 |= 0x050FFFF3; //P2.0,P2.2-P2.9,P2.12-P2.13

		/*
		 * P4[28] = LCDVD[6]/
		 * 			LCDVD[10]/
		 * 			LCDVD[2]		10
		 * P4[29] = LCDVD[7]/
		 * 			LCDVD[11]/
		 * 			LCDVD[3]		10
		 *
		 */
		pinsel = PINSEL9;
		pinsel &= 0xF0FFFFFF;
		pinsel |= 0x0A000000;
		PINSEL9 = pinsel;

		PINSEL10 = 0;

		//lcd = new LCDControllerDriver(LCD);

		// Board specific implementation
		lcd = new BlueScreenSunLCD(LCD);
	}
	return lcd;
}

/**
 * Get the Digital to Analog Converter.
 *
 * @return An instance to the Digital to Analog Converter interface
 */
DAC* LPC2478::getDAC() {
	if(dac == 0) {
		uint32_t  pinsel;

		pinsel = PINSEL1;
		pinsel &= ~0x00300000;
		pinsel |= 0x00200000;	/* Set P0.26 to AOUT (10) */
		PINSEL1 = pinsel;

		dac = new DAC(DAC0);
	}
	return dac;
}

/**
 * Get the General Purpose DMA channel 0 of the DMA Controller.
 *
 * @return DMA channel 0
 */
DMAChannel* LPC2478::getDMA0() {
	if(dma0 == 0) {
		PCONP |= (1 << 29);	// Enable GPDMA clock

		GPDMA->GPDMA_CONFIG |= 1;	// Enable GPDMA

		dma0 = new DMAChannel(0, GPDMA_CH0);
	}
	return dma0;
}

DMAChannel* LPC2478::getDMA1() {
	if(dma1 == 0) {
		PCONP |= (1 << 29);	// Enable GPDMA clock

		GPDMA->GPDMA_CONFIG |= 1;	// Enable GPDMA

		dma1 = new DMAChannel(0, GPDMA_CH1);
	}
	return dma1;
}

Timer* LPC2478::getTimer0() {
	if(timer0 == 0) {
		// Send power
		PCONP |= (1<<1);	// PCTIM0

		timer0 = new Timer(TIMER0, 0);
	}
	return timer0;
}

Timer* LPC2478::getTimer1() {
	if(timer1 == 0) {
		// Send power
		PCONP |= (1<<2);	// PCTIM1

		timer1 = new Timer(TIMER1, 1);
	}
	return timer1;
}

Timer* LPC2478::getTimer2() {
	if(timer2 == 0) {
		// Send power
		PCONP |= (1<<22);	// PCTIM2

		timer2 = new Timer(TIMER2, 2);
	}
	return timer2;
}

Timer* LPC2478::getTimer3() {
	if(timer3 == 0) {
		// Send power
		PCONP |= (1<<23);	// PCTIM3

		timer3 = new Timer(TIMER3, 3);
	}
	return timer3;
}

/**
 * Delay function.
 *
 * Configured for a 72 Mhz cclk.
 *
 * @param usec Length of the delay in microseconds
 */
void LPC2478::delay(uint32_t usec) {
	// Crazy unaccurate function for now
	//TODO: High priority fix needed here
	for(uint32_t i=0; i<usec; i++) {
		//for(uint32_t j=0; j<1; j++);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
	}
}
