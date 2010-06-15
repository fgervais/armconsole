/*
 * HostControllerDriver.h
 *
 *  Created on: Jan 22, 2010
 *      Author: fgervais
 */

#ifndef HOSTCONTROLLERDRIVER_H_
#define HOSTCONTROLLERDRIVER_H_

/*
 * All this is highly inspired of NXP USB Host Stack and
 * the Linux implementation of OHCI.
 */

#define USB_MEMORY 0x7FD00000

#include "LPC23xx.h"
#include <stdint.h>

class UsbDevice;

// Completion codes
#define CC_NOERROR	0

// Transaction tokens
#define PID_SETUP	0x00
#define PID_OUT		0x01
#define PID_IN		0x02

struct HcEd {                       	/* ----------- HostController EndPoint Descriptor ------------- */
    volatile  uint32_t  Control;		/* Endpoint descriptor control                              */
    volatile  uint32_t  TailTd;			/* Physical address of tail in Transfer descriptor list     */
    volatile  uint32_t  HeadTd;			/* Physcial address of head in Transfer descriptor list     */
    volatile  uint32_t  Next;			/* Physical address of next Endpoint descriptor             */
};

struct HcTd {                       	/* ------------ HostController Transfer Descriptor ------------ */
    volatile  uint32_t  Control;		/* Transfer descriptor control                              */
    volatile  uint32_t  CurrBufPtr;		/* Physical address of current buffer pointer               */
    volatile  uint32_t  Next;			/* Physical pointer to next Transfer Descriptor             */
    volatile  uint32_t  BufEnd;			/* Physical address of end of buffer                        */
};

struct Hcca {                      	 	/* ----------- Host Controller Communication Area ------------  */
    volatile  uint32_t  IntTable[32];	/* Interrupt Table                                          */
    volatile  uint32_t  FrameNumber;	/* Frame Number                                             */
    volatile  uint32_t  DoneHead;		/* Done Head                                                */
    volatile  uint32_t  Reserved[116];	/* Reserved for future use                                  */
    volatile  uint32_t  Unknown[4];		/* Unused                                                   */
};

struct RootHubPort {
	UsbDevice* device;
	// Port flags
	uint8_t deviceConnected;
	uint8_t deviceEnumerated;
};

class HostControllerDriver {
public:
	HostControllerDriver(OHCI_Typedef* ohciRegisters);
	virtual ~HostControllerDriver();

	uint8_t deviceConnected(uint32_t hubPortNumber) { return rootHubPort[hubPortNumber].deviceConnected; }
	uint8_t deviceEnumerated(uint32_t hubPortNumber) { return rootHubPort[hubPortNumber].deviceEnumerated; }
	void enumerateDevice(uint32_t hubPortNumber);

	void hcInterrupt();
private:
	Hcca* hcca;
	HcTd* headTd;
	HcTd* tailTd;
	HcEd* ctrlEd;
	HcEd* intInEd;
	HcEd* intOutEd;
	uint8_t* tdBuffer;
	uint8_t* userBuffer;

	OHCI_Typedef* ohciRegisters;

	volatile RootHubPort rootHubPort[MAX_ROOT_PORTS];

	// Global flags
	volatile uint8_t transferCompleted;

	void init();

	// USB control functions
	void getDescriptor(uint16_t descriptorTypeIndex, uint16_t descriptorLength, uint8_t* receiveBuffer);
	void setAddress();
	void setConfiguration();

	void controlRead(uint8_t bmRequestType,
			uint8_t bRequest,
			uint16_t wValue,
			uint16_t wIndex,
			uint16_t wLength,
			uint8_t* receiveBuffer);

	void controlWrite(uint8_t bmRequestType,
			uint8_t bRequest,
			uint16_t wValue,
			uint16_t wIndex,
			uint16_t wLength);

	uint8_t setupTransaction(HcEd* ed,
			uint8_t bmRequestType,
			uint8_t bRequest,
			uint16_t wValue,
			uint16_t wIndex,
			uint16_t wLength);

	uint8_t inTransaction(HcEd* ed, uint8_t* receiveBuffer, uint32_t transactionLength);
	uint8_t outTransaction(HcEd* ed, uint8_t* transmitBuffer, uint32_t transactionLength);

	uint8_t launchTransaction(HcEd* ed, uint32_t token, uint8_t* transmitBuffer, uint32_t transactionLength);

	void enqueueED(HcEd* ed);
	void enqueueTD(HcTd* td);
};

#endif /* HOSTCONTROLLERDRIVER_H_ */
