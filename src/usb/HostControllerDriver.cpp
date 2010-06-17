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
#include "GpioPin.h"
#include "Gpio.h"

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
	hcca = (Hcca *)(USB_MEMORY + 0x00);

	// Memory allocation inside the USB section
	headTd = (HcTd *)(USB_MEMORY+0x100);
	tailTd = (HcTd *)(USB_MEMORY+0x110);
	ctrlEd = (HcEd *)(USB_MEMORY+0x120);
	intInEd = (HcEd *)(USB_MEMORY+0x130);
	intOutEd = (HcEd *)(USB_MEMORY+0x140);
	tdBuffer = (uint8_t *)(USB_MEMORY+0x150);
	userBuffer = (uint8_t *)(USB_MEMORY+0x1000);

	// Clear USB memory
	uint8_t* memory_ptr = (uint8_t*)(USB_MEMORY + 0x00);
	for(uint32_t i=0; i<0x1000; i++) {
		*(memory_ptr+i) = 0x00;
	}

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

	ohciRegisters->HcPeriodicStart = (FRAME_INTERVAL * 9) / 10;
	ohciRegisters->HcFmInterval = ((((6 * (FRAME_INTERVAL - 210)) / 7) << 16) | FRAME_INTERVAL);

	// Software reset is completed within 10 us

	// We are now in the UsbSuspend state and must not stay in it for
	// more than 2 ms or the UsbResume state will have to be entered.

	// Switch to UsbOperational state
	ohciRegisters->HcControl =
		((ohciRegisters->HcControl & ~OHCI_CTRL_HCFS) | OHCI_USB_OPER);

	// Turn on power to all ports
	ohciRegisters->HcRh.Status |= RH_HS_LPSC;

	// A delay is needed here.
	LPC2478::delay(50000);	// 50 ms

	if(ohciRegisters->HcRevision == 0x10) {
		Debug::writeLine("OHCI Revision 1.0");
	}

	// Set HCCA
	ohciRegisters->HcHCCA = (uint32_t)hcca;

	// Clear any active interrupt
	((volatile OHCI_Typedef*)ohciRegisters)->HcInterruptStatus |= ohciRegisters->HcInterruptStatus;

	// Enable interrupts
	ohciRegisters->HcInterruptEnable =
		OHCI_INTR_MIE
		| OHCI_INTR_WDH		/* writeback of done_head */
		| OHCI_INTR_RHSC	/* root hub status change */
		| OHCI_INTR_UE
		| OHCI_INTR_SO
		| OHCI_INTR_RD
		| OHCI_INTR_FNO
		| OHCI_INTR_OC;

	VICIntSelect &= ~(1 << USB_INT); // IRQ Category (Not FIQ)
	VICIntEnable |= (1 << USB_INT);
}

void HostControllerDriver::enumerateDevice(uint32_t hubPortNumber) {
	if(hubPortNumber < MAX_ROOT_PORTS) {
		if(rootHubPort[hubPortNumber].deviceConnected && !rootHubPort[hubPortNumber].deviceEnumerated) {
			Debug::writeLine("Enumerating device");
			// USB 2.0 spec says at least 50ms delay before port reset
			LPC2478::delay(100000);
			// Set port reset
			ohciRegisters->HcRh.PortStatus[hubPortNumber] |= RH_PS_PRS;
			// Wait until reset is done
			while(ohciRegisters->HcRh.PortStatus[hubPortNumber] & RH_PS_PRS)
				Debug::writeLine("Port reset");
			Debug::writeLine("Port reset done");
			// Clear port reset status change
			ohciRegisters->HcRh.PortStatus[hubPortNumber] |= RH_PS_PRSC;
			LPC2478::delay(100000);

			// Get first 8 bytes of device descriptor

			// Endpoint 0, Address 0
			ctrlEd->Control = (8 << 16);	// Max packet size = 8 (Minimum value)
			getDescriptor(0x0100, 0x0008, userBuffer);	// device descriptor
			Debug::writeLine("Descriptor received");

			if(userBuffer[7] != 0) {
				Debug::writeLine("Got the max packet size");
				if(userBuffer[7] == 8) {
					Debug::writeLine("Got the right packet size");
				}
			}

		}
	}
}

void HostControllerDriver::getDescriptor(uint16_t descriptorTypeIndex, uint16_t descriptorLength, uint8_t* receiveBuffer) {
	controlRead(0x80, 0x06, descriptorTypeIndex, 0x0000, descriptorLength, receiveBuffer);
}

void HostControllerDriver::controlRead(uint8_t bmRequestType, uint8_t bRequest,
		uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t* receiveBuffer) {

	uint8_t completionCode;

	// Setup stage
	Debug::writeLine("Starting setup stage");
	completionCode = setupTransaction(ctrlEd, bmRequestType, bRequest, wValue, wIndex, wLength);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in setup stage");
	}
	// Data stage
	Debug::writeLine("Starting data stage");
	completionCode = inTransaction(ctrlEd, receiveBuffer, wLength);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in data stage");
	}
	// Status stage
	Debug::writeLine("Starting status stage");
	completionCode = outTransaction(ctrlEd, (uint8_t*)0, 0);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in status stage");
	}
}

uint8_t HostControllerDriver::setupTransaction(HcEd* ed, uint8_t bmRequestType, uint8_t bRequest,
		uint16_t wValue, uint16_t wIndex, uint16_t wLength) {

	// Build the request packet
	tdBuffer[0] = bmRequestType;
	tdBuffer[1] = bRequest;
	tdBuffer[2] = (uint8_t)(wValue & 0x00FF);
	tdBuffer[3] = (uint8_t)((wValue >> 8) & 0x00FF);
	tdBuffer[4] = (uint8_t)(wIndex & 0x00FF);
	tdBuffer[5] = (uint8_t)((wIndex >> 8) & 0x00FF);
	tdBuffer[6] = (uint8_t)(wLength & 0x00FF);
	tdBuffer[7] = (uint8_t)((wLength >> 8) & 0x00FF);

	return launchTransaction(ed, PID_SETUP, tdBuffer, 8);
}

uint8_t HostControllerDriver::inTransaction(HcEd* ed, uint8_t* receiveBuffer, uint32_t transactionLength) {
	return launchTransaction(ed, PID_IN, receiveBuffer, transactionLength);
}

uint8_t HostControllerDriver::outTransaction(HcEd* ed, uint8_t* transmitBuffer, uint32_t transactionLength) {
	return launchTransaction(ed, PID_OUT, transmitBuffer, transactionLength);
}

uint8_t HostControllerDriver::launchTransaction(HcEd* ed, uint32_t token, uint8_t* transmitBuffer, uint32_t transactionLength) {
	Debug::writeLine("Launching transaction");

	uint32_t dataToggle = ((token == PID_SETUP) ? 2 : 3);

	headTd->Control = (1 << 18)	// Data packet may be smaller than the buffer
		| (token << 19)			// PID
		| (dataToggle << 24)	// Data toggle = LSB of this field
		| (0x0F << 28);			// See section 4.3.3 of OHCI 1.0a specification
	tailTd->Control = 0;

	headTd->CurrBufPtr = (uint32_t)transmitBuffer;
	tailTd->CurrBufPtr = 0;
	headTd->Next = (uint32_t)tailTd;
	tailTd->Next = 0;
	headTd->BufEnd = (uint32_t)(transmitBuffer + (transactionLength - 1));
	tailTd->BufEnd = 0;

	ed->HeadTd = (uint32_t)headTd;
	ed->TailTd = (uint32_t)tailTd;
	ed->Next = 0;

	transferCompleted = 0;

	ohciRegisters->HcControlHeadED = (uint32_t)ed;
	//if(token == PID_SETUP) {
		ohciRegisters->HcCommandStatus |= (1<<1);	// Control list filled
		ohciRegisters->HcControl       |= (1<<4);	// Control list Enabled
	//}

	//while(ohciRegisters->HcCommandStatus & (1<<1));
	//Debug::writeLine("Control list processing started");

	// Wait until transfer is completed
	Debug::writeLine("Waiting for transfer completion");
	while(!transferCompleted);

	// Return completion code
	return (headTd->Control >> 28) & 0x0F;
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

	}
	// Writeback done head
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_WDH) {
		Debug::writeLine("Transfer completed");
		transferCompleted = 1;
	}
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_FNO) {
		Debug::writeLine("Frame number overflow");
	}
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_OC) {
		Debug::writeLine("Ownership change");
	}
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_RD) {
		Debug::writeLine("Resume detect");
	}
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_SO) {
		Debug::writeLine("Scheduling overrun");
	}
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_UE) {
		Debug::writeLine("Unrecoverable error");
	}
	else {
		Debug::writeLine("Unknown root hub interrupt source");
	}

	// Clear any active interrupt
	((volatile OHCI_Typedef*)ohciRegisters)->HcInterruptStatus |= ohciRegisters->HcInterruptStatus;

}
