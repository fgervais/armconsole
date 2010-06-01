/*
 * HostControllerDriver.cpp
 *
 *  Created on: Jan 22, 2010
 *      Author: fgervais
 */

#include "HostControllerDriver.h"
#include "LPC2478.h"
#include "UsbDevice.h"
#include "Debug.h"
#include "irq.h"

HostControllerDriver::HostControllerDriver(OHCI_Typedef* ohciRegisters) {
	this->ohciRegisters = ohciRegisters;

	//rootHubPort = new RootHubPort[MAX_ROOT_PORTS];

	// Reset root hub ports
	for(uint32_t hubPortNumber=0; hubPortNumber<MAX_ROOT_PORTS; hubPortNumber++) {
		rootHubPort[hubPortNumber].device = 0;
		rootHubPort[hubPortNumber].deviceConnected = 0;
		rootHubPort[hubPortNumber].deviceEnumerated = 0;
	}

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

	LPC2478::delay(50000);	// 50 ms

	/*
	 * Switch to UsbReset State
	 *
	 * This is also called "Hardware reset"
	 */
	ohciRegisters->HcControl = 0;

	ohciRegisters->HcBulkHeadED = 0;
	ohciRegisters->HcControlHeadED = 0;

	// Initiate a software reset
	ohciRegisters->HcCommandStatus |= OHCI_HCR;

	// Software reset is completed within 10 us

	// Software reset reset HcFmInterval register to the default (0x2EDF)

	// We are now in the UsbSuspend state and must not stay in it for
	// more than 2 ms or the UsbResume state will have to be entered.

	// Switch to UsbOperational state
	ohciRegisters->HcControl =
		((ohciRegisters->HcControl & ~OHCI_CTRL_HCFS) | OHCI_USB_OPER);

	// Turn on power to all ports
	ohciRegisters->HcRh.Status |= RH_HS_LPSC;

	// A delay is needed here.
	LPC2478::delay(50000);	// 50 ms

	// Set HCCA
	ohciRegisters->HcHCCA = (uint32_t)hcca;

	// Clear any active interrupt
	((volatile OHCI_Typedef*)ohciRegisters)->HcInterruptStatus |= ohciRegisters->HcInterruptStatus;

	// Enable interrupts
	ohciRegisters->HcInterruptEnable =
		OHCI_INTR_MIE
		| OHCI_INTR_WDH		/* writeback of done_head */
		| OHCI_INTR_RHSC;	/* root hub status change */

	VICIntSelect &= ~(1 << USB_INT); // IRQ Category (Not FIQ)
	VICIntEnable |= (1 << USB_INT);
}

void HostControllerDriver::enumerateDevice(uint32_t hubPortNumber) {

}

/**
 * Host Controller interrupt handler.
 *
 * This is called by the C interrupt wrapper is case of
 * a USB interrupt.
 */
void HostControllerDriver::hcInterrupt() {
	// Change to the root hub status registers
	if(ohciRegisters->HcInterruptStatus & OHCI_INTR_RHSC) {
		for(uint32_t hubPortNumber=0; hubPortNumber<MAX_ROOT_PORTS; hubPortNumber++) {
			// Connect status change
			if(ohciRegisters->HcRh.PortStatus[hubPortNumber] & RH_PS_CSC) {
				// Acknowledge status change (clear change bit)
				ohciRegisters->HcRh.PortStatus[hubPortNumber] |= RH_PS_CSC;

				// Device connected
				if(ohciRegisters->HcRh.PortStatus[hubPortNumber] & RH_PS_CCS) {
					Debug::writeLine("Device connected");
					rootHubPort[hubPortNumber].deviceConnected = 1;
					rootHubPort[hubPortNumber].deviceEnumerated = 0;

					// Low speed device attached
					if(ohciRegisters->HcRh.PortStatus[hubPortNumber] & RH_PS_LSDA) {
						Debug::writeLine("Low speed device");
					}
					else {
						Debug::writeLine("Full speed device");
					}
				}
				// Device disconnected
				else {
					Debug::writeLine("Device disconnected");
					rootHubPort[hubPortNumber].deviceConnected = 0;
					rootHubPort[hubPortNumber].deviceEnumerated = 0;
				}
			}
		}

		/*
		// Connect status change on port 0
		if(ohciRegisters->HcRh.PortStatus[0] & RH_PS_CSC) {
			// Acknowledge status change (clear change bit)
			ohciRegisters->HcRh.PortStatus[0] |= RH_PS_CSC;

			// Device connected
			if(ohciRegisters->HcRh.PortStatus[0] & RH_PS_CCS) {
				Debug::writeLine("Device connected on port 0");
				LPC2478::delay(1000000);

				// Low speed device attached
				if(ohciRegisters->HcRh.PortStatus[0] & RH_PS_LSDA) {
					Debug::writeLine("Low speed device");
					LPC2478::delay(1000000);
				}
				else {
					Debug::writeLine("Full speed device");
					LPC2478::delay(1000000);
				}
			}
			// Device disconnected
			else {
				Debug::writeLine("Device disconnected on port 0");
				LPC2478::delay(1000000);
			}
		}
		// Connect status change on port 1
		else if(ohciRegisters->HcRh.PortStatus[1] & RH_PS_CSC) {
			// Acknowledge status change (clear change bit)
			ohciRegisters->HcRh.PortStatus[1] |= RH_PS_CSC;

			// Device connected
			if(ohciRegisters->HcRh.PortStatus[1] & RH_PS_CCS) {
				Debug::writeLine("Device connected on port 1");
				LPC2478::delay(1000000);

				// Low speed device attached
				if(ohciRegisters->HcRh.PortStatus[1] & RH_PS_LSDA) {
					Debug::writeLine("Low speed device");
					LPC2478::delay(1000000);
				}
				else {
					Debug::writeLine("Full speed device");
					LPC2478::delay(1000000);
				}
			}
			// Device disconnected
			else {
				Debug::writeLine("Device disconnected on port 1");
				LPC2478::delay(1000000);
			}
		}
		else {
			Debug::writeLine("Unknown root hub interrupt source");
			LPC2478::delay(1000000);
		}*/

	}
	else {
		Debug::writeLine("Unknown root hub interrupt source");
	}

	// Clear any active interrupt
	((volatile OHCI_Typedef*)ohciRegisters)->HcInterruptStatus |= ohciRegisters->HcInterruptStatus;

}
