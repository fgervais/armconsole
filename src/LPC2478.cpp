/*
 * LPC2478.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "LPC2478.h"
#include "Gpio.h"
#include "HostControllerDriver.h"

/* Static initialization. Required to make the compiler happy */
Gpio* LPC2478::gpio0 = 0;
HostControllerDriver* LPC2478::hcd = 0;

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
		gpio0 = new Gpio(FIO0, 32, 0);
	}
	return gpio0;
}

/**
 * Used to access USB Host Controller Driver.
 *
 * @return Reference to the Host Controller Driver
 */
HostControllerDriver* LPC2478::getHCD() {
	if(hcd == 0) {
		hcd = new HostControllerDriver(OHCI);

		uint32_t  pinsel;
		uint32_t  pinmode;


		PCONP |= 0x80000000;
		// Enable the USB interrupt source
		VICIntEnClr    = (1 << 22);
		// Enable USB host clock
		OTG_CLK_CTRL   = 0x00000001;
		// Wait until USB host clock is stable
		while ((OTG_CLK_STAT & 0x00000001) == 0);

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
	}
	return hcd;
}