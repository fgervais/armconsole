/*
 * HostControllerDriver.cpp
 *
 *  Created on: Jan 22, 2010
 *      Author: fgervais
 */

#include "HostControllerDriver.h"
#include "irq.h"

HostControllerDriver::HostControllerDriver(OHCI_Typedef* ohciRegisters) {
	this->ohciRegisters = ohciRegisters;

	init();

}

HostControllerDriver::~HostControllerDriver() {

}

void HostControllerDriver::init() {
	/*
	 * Allocate the Host Controller Communication Area
	 *
	 * It must be 256-bytes aligned.
	 */
	hcca = (Hcca *)0x7FD00000;

	// Clear the HCCA
	for(uint8_t i=0; i<32; i++) {
		hcca->IntTable[i] = 0;
	}
	hcca->DoneHead = 0;
	hcca->FrameNumber = 0;

	/*
	 * Switch to UsbReset State
	 *
	 * This is also called "Hardware reset"
	 */
	ohciRegisters->HcControl = 0;

	// Initiate a software reset
	ohciRegisters->HcCommandStatus |= OHCI_HCR;

	// Let the default (0x2EDF) Frame Interval

	// Switch to UsbOperational state
	ohciRegisters->HcControl =
		((ohciRegisters->HcControl & ~OHCI_CTRL_HCFS) | OHCI_USB_OPER);

	// Turn off power to all ports
	ohciRegisters->HcRh.Status |= RH_HS_LPSC;

	// Set HCCA
	ohciRegisters->HcHCCA = (uint32_t)hcca;

	// Clear any active interrupt
	ohciRegisters->HcInterruptStatus |= ohciRegisters->HcInterruptStatus;

	// Enable interrupts
	ohciRegisters->HcInterruptEnable =
		OHCI_INTR_WDH		/* writeback of done_head */
		| OHCI_INTR_RHSC;	/* root hub status change */

	VICIntEnable = 1 << USB_INT;
	VICIntSelect &= ~(1 << USB_INT); // IRQ Category (Not FIQ)

	// Master interrupt enable
	ohciRegisters->HcInterruptEnable |= OHCI_INTR_MIE;
}
