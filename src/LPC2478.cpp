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
#include "irq.h"

/* Static initialization. Required to make the compiler happy */
Gpio* LPC2478::gpio0 = 0;
Gpio* LPC2478::gpio1 = 0;
HostControllerDriver* LPC2478::hcd = 0;
LCDControllerDriver* LPC2478::lcd = 0;

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
 * Used to access General Purpose IO port 0.
 *
 * @return Reference to IO port 0
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
		PCONP |= (1 << 20); //power on LCD controller

		// Time delay?

		//TODO: Clear before set
		PINSEL0 |= 0x00055500; //P0.4-P0.9
		PINSEL3 |= 0x05555500; //P1.20-P1.29
		PINSEL4 |= 0x050FFFF3; //P2.0,P2.2-P2.9,P2.12-P2.13
		PINSEL9 |= 0x0A000000; //P4.28-P4.29

		PINSEL10 = 0;
		PINSEL11 = 0x0000000F; //24-bit mode

		lcd = new LCDControllerDriver();
	}
	return lcd;
}
