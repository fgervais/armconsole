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
#include "UsbDevice.h"
#include "DeviceDescriptor.h"
#include "ConfigurationDescriptor.h"
#include "InterfaceDescriptor.h"
#include "EndpointDescriptor.h"
#include <cstdio>

HostControllerDriver::HostControllerDriver(OHCI_Typedef* ohciRegisters) {
	this->ohciRegisters = ohciRegisters;

	//rootHubPort = new RootHubPort[MAX_ROOT_PORTS];

	// Reset root hub ports
	for(uint32_t hubPortNumber=0; hubPortNumber<MAX_ROOT_PORTS; hubPortNumber++) {
		rootHubPort[hubPortNumber].device = 0;
		rootHubPort[hubPortNumber].deviceConnected = 0;
		rootHubPort[hubPortNumber].deviceEnumerated = 0;
		rootHubPort[hubPortNumber].lowSpeed = 0;
		// It
		rootHubPort[hubPortNumber].deviceAddressesStart = hubPortNumber + 1;
	}

	// Initialize devices
	for(uint32_t deviceNumber=0; deviceNumber<MAXIMUM_NUMBER_OF_DEVICE; deviceNumber++) {
		device[deviceNumber] = 0;
	}

	init();
}

HostControllerDriver::~HostControllerDriver() {
	/* Free memory */
	for(uint32_t deviceNumber=0; deviceNumber<MAXIMUM_NUMBER_OF_DEVICE; deviceNumber++) {
		if(device[deviceNumber] != 0) {
			delete device[deviceNumber];
		}
	}
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
	ohciRegisters->HcLSThreshold = 0x628;

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
		//| OHCI_INTR_FNO
		| OHCI_INTR_OC;

	VICIntSelect &= ~(1 << USB_INT); // IRQ Category (Not FIQ)
	VICIntEnable |= (1 << USB_INT);
}

void HostControllerDriver::portReset(uint32_t hubPortNumber) {
	// Set port reset
	ohciRegisters->HcRh.PortStatus[hubPortNumber] |= RH_PS_PRS;
	// Wait until reset is done
	while(ohciRegisters->HcRh.PortStatus[hubPortNumber] & RH_PS_PRS);
	// Clear port reset status change
	ohciRegisters->HcRh.PortStatus[hubPortNumber] |= RH_PS_PRSC;
	LPC2478::delay(100000);
}

void HostControllerDriver::enumerateDevice(uint32_t hubPortNumber) {
	uint8_t completionCode;
	uint8_t timeout;

	if(hubPortNumber < MAX_ROOT_PORTS) {
		if(rootHubPort[hubPortNumber].deviceConnected && !rootHubPort[hubPortNumber].deviceEnumerated) {
			Debug::writeLine("Enumerating device");
			// USB 2.0 spec says at least 50ms delay before port reset
			LPC2478::delay(100000);
			portReset(hubPortNumber);

			// Get first 8 bytes of device descriptor

			// Endpoint 0, Address 0
			ctrlEd->Control = (8 << 16);	// Max packet size = 8 (Minimum value)
			if(rootHubPort[hubPortNumber].lowSpeed) {
				ctrlEd->Control |= (1 << 13);	// Low speed endpoint
			}
			completionCode = getDescriptor(DEVICE_DESCRIPTOR_INDEX, 0x0012, userBuffer);
			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Descriptor received");
			}
			else {
				Debug::writeLine("Error with get_descriptor request");
				return;
			}

			// Set the maximum packet size
			ctrlEd->Control = (userBuffer[7] << 16);
			if(rootHubPort[hubPortNumber].lowSpeed) {
				ctrlEd->Control |= (1 << 13);	// Low speed endpoint
			}

			// Reset the device to be in a known state
			portReset(hubPortNumber);

			// Create the new logical device
			rootHubPort[hubPortNumber].device = new UsbDevice(hubPortNumber);

			// Get the whole device descriptor
			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Getting device descriptor");;
				completionCode = getDescriptor(DEVICE_DESCRIPTOR_INDEX, DEVICE_DESCRIPTOR_LENGTH, userBuffer);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Device descriptor received");
			}
			else {
				Debug::writeLine("Error with get_descriptor request");
				return;
			}

			rootHubPort[hubPortNumber].device->setDeviceDescriptor(new DeviceDescriptor(userBuffer));

			if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idVendor == 0x045e) { // Xbox receiver
				if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idProduct == 0x0719) { // Xbox receiver
					Debug::writeLine("Xbox wireless receiver connected");
				}
			}
			else if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idVendor == 0x046d) { // Mouse
				if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idProduct == 0xc018) { // Mouse
					Debug::writeLine("Logitech mouse connected");
				}
			}
			else {
				Debug::writeLine("Unknown USB device");
			}

			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Setting address");
				completionCode = setAddress(rootHubPort[hubPortNumber].deviceAddressesStart);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Address set");
			}
			else {
				Debug::writeLine("Error with set_address");
				return;
			}

			// Set the new address
			ctrlEd->Control |= (rootHubPort[hubPortNumber].deviceAddressesStart);

			// Device must finish the Set Address request within 2ms
			LPC2478::delay(2000);

			// Get the configuration descriptor
			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Getting configuration descriptor");
				completionCode = getDescriptor(CONFIGURATION_DESCRIPTOR_INDEX, 0x09, userBuffer);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Configuration descriptor received");
			}
			else {
				Debug::writeLine("Error with get_descriptor request");
				return;
			}
			// We queried the configuration descriptor only to get this value
			uint16_t wTotalLength = (userBuffer[3] << 8) | userBuffer[2];

			// Now get the whole configuration descriptor tree
			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Getting configuration tree");
				completionCode = getDescriptor(CONFIGURATION_DESCRIPTOR_INDEX, wTotalLength, userBuffer);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Configuration tree received");
			}
			else {
				Debug::writeLine("Error with get_descriptor request");
				return;
			}

			// Set the new configuration tree to the device informations
			rootHubPort[hubPortNumber].device->setConfigurationDescriptor(new ConfigurationDescriptor(userBuffer));

			// Show debug informations
			printDescriptors(rootHubPort[hubPortNumber].device);

			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Setting configuration");
				completionCode = setConfiguration(0x01);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Configuration set");
			}
			else {
				Debug::writeLine("Error with set_configuration");
				return;
			}

			rootHubPort[hubPortNumber].deviceEnumerated = 1;
		}
	}
}

void HostControllerDriver::periodicTask() {
	for(uint32_t hubPortNumber=0; hubPortNumber<MAX_ROOT_PORTS; hubPortNumber++) {
		// Device disconnected
		if(!rootHubPort[hubPortNumber].deviceConnected && rootHubPort[hubPortNumber].device != 0) {
			delete rootHubPort[hubPortNumber].device;
			rootHubPort[hubPortNumber].device = 0;
		}
		else if(rootHubPort[hubPortNumber].deviceConnected && !rootHubPort[hubPortNumber].deviceEnumerated) {
			enumerateDevice(hubPortNumber);
		}
	}
}

uint8_t HostControllerDriver::getDescriptor(uint16_t descriptorTypeIndex, uint16_t descriptorLength, uint8_t* receiveBuffer) {
	return controlRead(0x80, 0x06, descriptorTypeIndex, 0x0000, descriptorLength, receiveBuffer);
}

uint8_t HostControllerDriver::setAddress(uint16_t address) {
	return controlWrite(0x00, 0x05, address, 0x0000, 0x0000);
}

uint8_t HostControllerDriver::setConfiguration(uint8_t configuration) {
	return controlWrite(0x00, 0x09, configuration, 0x0000, 0x0000);
}

uint8_t HostControllerDriver::controlRead(uint8_t bmRequestType, uint8_t bRequest,
		uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t* receiveBuffer) {

	uint8_t completionCode;

	// Setup stage
	completionCode = setupTransaction(ctrlEd, bmRequestType, bRequest, wValue, wIndex, wLength);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in setup stage");
		return completionCode;
	}
	// Data stage
	completionCode = inTransaction(ctrlEd, receiveBuffer, wLength);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in data stage");
		return completionCode;
	}
	// Status stage
	completionCode = outTransaction(ctrlEd, (uint8_t*)0, 0);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in status stage");
		return completionCode;
	}
	return completionCode;
}

uint8_t HostControllerDriver::controlWrite(uint8_t bmRequestType, uint8_t bRequest,
		uint16_t wValue, uint16_t wIndex, uint16_t wLength) {

	uint8_t completionCode;

	// Setup stage
	completionCode = setupTransaction(ctrlEd, bmRequestType, bRequest, wValue, wIndex, wLength);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in setup stage");
		return completionCode;
	}
	// Status stage
	/*
	 * The status stage should always be in the opposite direction of the last
	 * transaction. As there is not Data Stage, the last transaction was in the
	 * OUT direction so the Status Stage send an IN token.
	 * We should get NAK or the DATA1 PID. When we get the DATA1 PID the controller
	 * automatically sends the closing ACK.
	 */
	completionCode = inTransaction(ctrlEd, (uint8_t*)0, 0);
	if(completionCode != CC_NOERROR) {
		Debug::writeLine("Error in status stage");
		return completionCode;
	}
	return completionCode;
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
	// TODO: A lot of that stuff doesn't work for something else than control transactions

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

	// Wait until transfer is completed
	while(!transferCompleted);

	// Return completion code
	return (headTd->Control >> 28) & 0x0F;
}

void HostControllerDriver::printDescriptors(UsbDevice* device) {
	char buffer[80];

	Debug::writeLine("Configuration: 1");

	sprintf(buffer,"- Interfaces: %d",device->getConfigurationDescriptor()->bNumInterfaces);
	Debug::writeLine(buffer);

	for(uint8_t i=0; i<device->getConfigurationDescriptor()->bNumInterfaces; i++) {
		sprintf(buffer,"  Interface: %d",i);
		Debug::writeLine(buffer);

		sprintf(buffer,"  - Endpoints: %d",device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->bNumEndPoints);
		Debug::writeLine(buffer);

		for(int j=0; j<device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->bNumEndPoints; j++) {
			sprintf(buffer,"  - Endpoint: %d %s %s MaxPacket %d Interval %d ms",(device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bEndpointAddress & 0x0F),
					((device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bEndpointAddress & 0xF0) >> 7) == 1 ? "IN" : "OUT",
					(device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bmAttributes & 0x03) ? "Interrupt" : "Other",
					device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->wMaxPacketSize,
					device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bInterval);
			Debug::writeLine(buffer);
		}
	}
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
						rootHubPort[hubPortNumber].lowSpeed = 1;
					}
					else {
						Debug::writeLine("Full speed device");
						rootHubPort[hubPortNumber].lowSpeed = 0;
					}
				}
				// Device disconnected
				else {
					Debug::writeLine("Device disconnected");
					rootHubPort[hubPortNumber].deviceConnected = 0;
					rootHubPort[hubPortNumber].deviceEnumerated = 0;
					rootHubPort[hubPortNumber].lowSpeed = 0;
				}
			}
		}

	}
	// Writeback done head
	else if(ohciRegisters->HcInterruptStatus & OHCI_INTR_WDH) {
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
