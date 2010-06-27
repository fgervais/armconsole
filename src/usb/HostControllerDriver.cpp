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
		// The HCD will start addressing devices at this address number
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
	tailTd = (HcTd *)(USB_MEMORY+0x120);
	ctrlEd = (HcEd *)(USB_MEMORY+0x140);
	tdBuffer = (uint8_t *)(USB_MEMORY+0x150);
	userBuffer = (uint8_t *)(USB_MEMORY+0x160);

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

	// Build the interrupt endpoint tree
	/*
	 * Every node of this tree has it's own pooling time.
	 *
	 * The array has this form :
	 *
	 * 0,1,2,2,4,4,4,4,8,8,8,8,8,8,8,8,...,16,...,32
	 *
	 * You can see that the 16 nodes with a polling time
	 * of 16 ms have a position starting at index 16 of
	 * the array. There may be a better way but this seems
	 * efficient enough.
	 */
	interruptTreeNode = (HcEd*)(USB_MEMORY + 0x400);

	// Clear every EDs (USB_MEMORY + 0x400) to (USB_MEMORY + 0x800)
	memory_ptr = (uint8_t*)(USB_MEMORY + 0x400);
	for(uint32_t i=0; i<0x400; i++) {
		*(memory_ptr+i) = 0x00;
	}

	// Set the skip bit for all of them
	for(uint8_t i=0; i<HCCA_INTERRUPT_NUMBER*2; i++) {
		interruptTreeNode[i].Control |= (1 << 14);
	}

	// Link all these structs in some king a reverse binary tree
	// See OHCI specification 1.0a 5.2.7.2
	/*
	 * NOTE: I created this algorithm by looking at the pattern
	 * myself. It is possible that there is known algorithm for
	 * this kind of problem but I'm not aware of them.
	 */
	for(uint8_t i=0; i<HCCA_INTERRUPT_NUMBER; i++) {
		interruptTreeNode[(i>>0) + 32].Next = (uint32_t)&interruptTreeNode[(i>>1) + 16];
		interruptTreeNode[(i>>1) + 16].Next = (uint32_t)&interruptTreeNode[(i>>2) + 8];
		interruptTreeNode[(i>>2) + 8].Next = (uint32_t)&interruptTreeNode[(i>>3) + 4];
		interruptTreeNode[(i>>3) + 4].Next = (uint32_t)&interruptTreeNode[(i>>4) + 2];
		interruptTreeNode[(i>>4) + 2].Next = (uint32_t)&interruptTreeNode[(i>>5) + 1];
		interruptTreeNode[(i>>5) + 1].Next = 0;
	}

	// Add all the leaf of the tree to the HCCA interrupt table
	for(uint8_t i=0; i<HCCA_INTERRUPT_NUMBER; i++) {
		hcca->IntTable[i] = (uint32_t)&interruptTreeNode[32 + revbits[i]];
	}

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

UsbDevice* HostControllerDriver::enumerateDevice(uint32_t hubPortNumber) {
	uint8_t completionCode;
	uint8_t timeout;

	if(hubPortNumber < MAX_ROOT_PORTS) {
		if(rootHubPort[hubPortNumber].deviceConnected && !rootHubPort[hubPortNumber].deviceEnumerated) {
			Debug::writeLine("Enumerating device");

			// Address that will be set to the device
			uint8_t deviceAddress = rootHubPort[hubPortNumber].deviceAddressesStart;

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
				return 0;
			}

			// Set the maximum packet size
			ctrlEd->Control = (userBuffer[7] << 16);
			if(rootHubPort[hubPortNumber].lowSpeed) {
				ctrlEd->Control |= (1 << 13);	// Low speed endpoint
			}

			// Reset the device to be in a known state
			portReset(hubPortNumber);

			// Create the new logical device
			rootHubPort[hubPortNumber].device = new UsbDevice(deviceAddress, rootHubPort[hubPortNumber].lowSpeed ? UsbDevice::LowSpeed : UsbDevice::FullSpeed);

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
				return 0;
			}

			rootHubPort[hubPortNumber].device->setDeviceDescriptor(new DeviceDescriptor(userBuffer));

			timeout = ENUMERATION_QUERY_TIMEOUT;
			do {
				Debug::writeLine("Setting address");
				completionCode = setAddress(deviceAddress);
			} while(completionCode != CC_NOERROR && --timeout > 0);

			if(completionCode == CC_NOERROR) {
				Debug::writeLine("Address set");
			}
			else {
				Debug::writeLine("Error with set_address");
				return 0;
			}

			// Set the new address
			ctrlEd->Control |= (deviceAddress);

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
				return 0;
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
				return 0;
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
				return 0;
			}

			rootHubPort[hubPortNumber].deviceEnumerated = 1;
		}
	}

	return rootHubPort[hubPortNumber].device;
}

UsbDevice* HostControllerDriver::periodicTask() {
	for(uint32_t hubPortNumber=0; hubPortNumber<MAX_ROOT_PORTS; hubPortNumber++) {
		// Device disconnected
		if(!rootHubPort[hubPortNumber].deviceConnected && rootHubPort[hubPortNumber].device != 0) {
			unregisterEndpoints(rootHubPort[hubPortNumber].device);
			delete rootHubPort[hubPortNumber].device;
			rootHubPort[hubPortNumber].device = 0;
		}
		else if(rootHubPort[hubPortNumber].deviceConnected && !rootHubPort[hubPortNumber].deviceEnumerated) {
			UsbDevice* device = enumerateDevice(hubPortNumber);
			if(device != 0) {
				Debug::writeLine("Entering registerEndpoints()");
				registerEndpoints(device);
			}
			return device;
		}
	}
	return 0;
}

void HostControllerDriver::usbRequest(UsbDevice* device, uint8_t interfaceIndex, uint8_t endpointIndex, uint8_t* transactionBuffer, uint32_t transactionLength) {
	HcEd* endpoint = device->getEndpoints()[interfaceIndex][endpointIndex];
	EndpointDescriptor* currentEndpointDescriptor = device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceIndex)->getEndpointDescriptor(endpointIndex);
	// The new tail TD
	HcTd* dummyTD = (HcTd*)(USB_MEMORY+0x800);

	switch((currentEndpointDescriptor->bmAttributes & 0x03)) {
	// Isochronous
	case 1:
		break;
	// Bulk
	case 2:
		break;
	// Interrupt
	case 3:
		ohciRegisters->HcControl &= ~(1<<2);	// Periodic list Disabled
		LPC2478::delay(1000);
		break;
	}

	((HcTd*)endpoint->TailTd)->Control = (1 << 18)	// Data packet may be smaller than the buffer
		| ((((currentEndpointDescriptor->bEndpointAddress & 0xF0) >> 7) == 1 ? PID_IN : PID_OUT) << 19)		// PID
		| (DATA1 << 24)			// Data toggle = LSB of this field
		| (0x0F << 28);			// See section 4.3.3 of OHCI 1.0a specification
	dummyTD->Control = 0;

	((HcTd*)endpoint->TailTd)->CurrBufPtr = (uint32_t)transactionBuffer;
	dummyTD->CurrBufPtr = 0;
	((HcTd*)endpoint->TailTd)->Next = (uint32_t)dummyTD;
	dummyTD->Next = 0;
	((HcTd*)endpoint->TailTd)->BufEnd = (uint32_t)(transactionBuffer + (transactionLength - 1));
	dummyTD->BufEnd = 0;

	((HcTd*)endpoint->TailTd)->automaticRequeue = 1;
	((HcTd*)endpoint->TailTd)->parentED = endpoint;

	endpoint->TailTd = (uint32_t)dummyTD;

	switch((currentEndpointDescriptor->bmAttributes & 0x03)) {
	// Isochronous
	case 1:
		break;
	// Bulk
	case 2:
		break;
	// Interrupt
	case 3:
		ohciRegisters->HcControl |= (1<<2);		// Periodic list Enabled
		break;
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

uint8_t HostControllerDriver::launchTransaction(HcEd* ed, uint32_t token, uint8_t* transactionBuffer, uint32_t transactionLength) {
	// TODO: A lot of that stuff doesn't work for something else than control transactions

	uint32_t dataToggle = ((token == PID_SETUP) ? DATA0 : DATA1);

	headTd->Control = (1 << 18)	// Data packet may be smaller than the buffer
		| (token << 19)			// PID
		| (dataToggle << 24)	// Data toggle = LSB of this field
		| (0x0F << 28);			// See section 4.3.3 of OHCI 1.0a specification
	tailTd->Control = 0;

	headTd->CurrBufPtr = (uint32_t)transactionBuffer;
	tailTd->CurrBufPtr = 0;
	headTd->Next = (uint32_t)tailTd;
	tailTd->Next = 0;
	headTd->BufEnd = (uint32_t)(transactionBuffer + (transactionLength - 1));
	tailTd->BufEnd = 0;

	// This is not a periodic transfer
	headTd->automaticRequeue = 0;

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

void HostControllerDriver::setupPeriodicIn() {

}

void HostControllerDriver::setupPeriodicOut(uint8_t* transmitBuffer, uint32_t transactionLength) {
	headTd->Control = (1 << 18)	// Data packet may be smaller than the buffer
		| (PID_OUT << 19)		// PID
		| (DATA1 << 24)			// Data toggle = LSB of this field
		| (0x0F << 28);			// See section 4.3.3 of OHCI 1.0a specification
	tailTd->Control = 0;

	headTd->CurrBufPtr = (uint32_t)transmitBuffer;
	tailTd->CurrBufPtr = 0;
	headTd->Next = (uint32_t)tailTd;
	tailTd->Next = 0;
	headTd->BufEnd = (uint32_t)(transmitBuffer + (transactionLength - 1));
	tailTd->BufEnd = 0;

	intOutEd->HeadTd = (uint32_t)headTd;
	intOutEd->TailTd = (uint32_t)tailTd;
	intOutEd->Next = 0;

	// Set 8ms interval
	hcca->IntTable[0] = (uint32_t)intOutEd;
	hcca->IntTable[7] = (uint32_t)intOutEd;
	hcca->IntTable[15] = (uint32_t)intOutEd;
	hcca->IntTable[31] = (uint32_t)intOutEd;

	ohciRegisters->HcControl       |= (1<<2);	// Periodic list Enabled
}

void HostControllerDriver::registerEndpoints(UsbDevice* device) {
	// TODO: Add some sort of dynamic memory allocation

	// Memory allocation
	HcEd*** endpoint;
	HcTd* dummyTD;

	// The first 1k of USB memory is reserved for device enumeration
	uint32_t usb_memory_ptr = USB_MEMORY + 0x1000;

	endpoint = new HcEd**[device->getConfigurationDescriptor()->bNumInterfaces];

	for(uint8_t interfaceNumber=0; interfaceNumber<device->getConfigurationDescriptor()->bNumInterfaces; interfaceNumber++) {
		endpoint[interfaceNumber] = new HcEd*[device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceNumber)->bNumEndPoints];

		for(uint8_t endpointNumber=0; endpointNumber<device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceNumber)->bNumEndPoints; endpointNumber++) {

			endpoint[interfaceNumber][endpointNumber] = (HcEd *)(usb_memory_ptr);
			usb_memory_ptr += sizeof(HcEd);

			dummyTD = (HcTd *)(usb_memory_ptr);
			usb_memory_ptr += sizeof(HcTd);

			dummyTD->Control = 0;
			dummyTD->CurrBufPtr = 0;
			dummyTD->Next = 0;
			dummyTD->BufEnd = 0;

			EndpointDescriptor* currentEndpointDescriptor = device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceNumber)->getEndpointDescriptor(endpointNumber);

			// Set the maximum packet size
			endpoint[interfaceNumber][endpointNumber]->Control = (currentEndpointDescriptor->wMaxPacketSize << 16);
			if(device->getSpeed() == UsbDevice::LowSpeed) {
				endpoint[interfaceNumber][endpointNumber]->Control |= (1 << 13);	// Low speed endpoint
			}
			// Device address
			endpoint[interfaceNumber][endpointNumber]->Control |= (device->getAddress());
			// Endpoint number
			endpoint[interfaceNumber][endpointNumber]->Control |= (currentEndpointDescriptor->bEndpointAddress << 7);

			endpoint[interfaceNumber][endpointNumber]->HeadTd = (uint32_t)dummyTD;
			endpoint[interfaceNumber][endpointNumber]->TailTd = (uint32_t)dummyTD;
			endpoint[interfaceNumber][endpointNumber]->Next = 0;

			// TODO: Register Bulk and isochronous endpoints as well
			switch((currentEndpointDescriptor->bmAttributes & 0x03)) {
			// Isochronous
			case 1:
				break;
			// Bulk
			case 2:
				break;
			// Interrupt
			case 3:
				// Find the interval next highest power of two
				uint8_t power2 = currentEndpointDescriptor->bInterval;

				// Protect against buggy devices
				if(power2 == 0) {
					power2 = 1;
				}

				power2--;
				power2 |= power2 >> 1;  // handle  2 bit numbers
				power2 |= power2 >> 2;  // handle  4 bit numbers
				power2 |= power2 >> 4;  // handle  8 bit numbers
				power2++;

				if(power2 > currentEndpointDescriptor->bInterval) {
					power2 >>= 1;
				}

				ohciRegisters->HcControl &= ~(1<<2);	// Periodic list Disabled
				// Ensure we finish the current frame processing
				LPC2478::delay(1000);

				// TODO: Add load balancing
				endpoint[interfaceNumber][endpointNumber]->Next = interruptTreeNode[power2].Next;
				interruptTreeNode[power2].Next = (uint32_t)endpoint[interfaceNumber][endpointNumber];

				ohciRegisters->HcControl |= (1<<2);		// Periodic list Enabled
				break;
			}

		}
	}

	device->setEndpoints(endpoint);
}

void HostControllerDriver::unregisterEndpoints(UsbDevice* device) {
	HcEd*** endpoint = device->getEndpoints();

	for(uint8_t interfaceNumber=0; interfaceNumber<device->getConfigurationDescriptor()->bNumInterfaces; interfaceNumber++) {
		for(uint8_t endpointNumber=0; endpointNumber<device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceNumber)->bNumEndPoints; endpointNumber++) {

			EndpointDescriptor* currentEndpointDescriptor = device->getConfigurationDescriptor()->getInterfaceDescriptor(interfaceNumber)->getEndpointDescriptor(endpointNumber);

			// TODO: Register Bulk and isochronous endpoints as well
			switch((currentEndpointDescriptor->bmAttributes & 0x03)) {
			// Isochronous
			case 1:
				break;
			// Bulk
			case 2:
				break;
			// Interrupt
			case 3:
				// Find the interval next highest power of two
				uint8_t power2 = currentEndpointDescriptor->bInterval;

				// Protect against buggy devices
				if(power2 == 0) {
					power2 = 1;
				}

				power2--;
				power2 |= power2 >> 1;  // handle  2 bit numbers
				power2 |= power2 >> 2;  // handle  4 bit numbers
				power2 |= power2 >> 4;  // handle  8 bit numbers
				power2++;

				// We found the highest power of 2 but we need the lowest
				if(power2 > 1) {
					power2 >>= 1;
				}

				ohciRegisters->HcControl &= ~(1<<2);	// Periodic list Disabled
				// Ensure we finish the current frame processing
				LPC2478::delay(1000);

				// Find the endpoint we want to unlink
				HcEd* endpointIterator = &interruptTreeNode[power2];
				while(endpointIterator->Next != (uint32_t)endpoint[interfaceNumber][endpointNumber]) {
					endpointIterator = (HcEd*)endpointIterator->Next;
				}
				// Unlink the endpoint from the list
				endpointIterator->Next = ((HcEd*)endpointIterator->Next)->Next;

				ohciRegisters->HcControl |= (1<<2);		// Periodic list Enabled
				break;
			}

		}
	}
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

		for(uint8_t j=0; j<device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->bNumEndPoints; j++) {
			sprintf(buffer,"  - Endpoint: %d %s %s MaxPacket %d Interval %d ms",(device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bEndpointAddress & 0x0F),
					((device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bEndpointAddress & 0xF0) >> 7) == 1 ? "IN" : "OUT",
					(device->getConfigurationDescriptor()->getInterfaceDescriptor(i)->getEndpointDescriptor(j)->bmAttributes & 0x03) == 3 ? "Interrupt" : "Other",
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
