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
#include "MemoryPool.h"
#include <stdint.h>

// Forward declaration
class UsbDevice;
class HCDRequest;
class HCDEventListener;

// Device descriptor
#define DEVICE_DESCRIPTOR_INDEX			0x0100
#define DEVICE_DESCRIPTOR_LENGTH		0x12
#define CONFIGURATION_DESCRIPTOR_INDEX	0x0200
#define CONFIGURATION_DESCRIPTOR_LENGTH	0x09

#define FRAME_INTERVAL		0x2EDF		/* 12000 bits per frame (-1) */

// Completion codes
#define CC_NOERROR	0

// Transaction tokens
#define PID_SETUP	0x00
#define PID_OUT		0x01
#define PID_IN		0x02

#define DATA0		0x02
#define DATA1		0x03

// The maximum number of time we will retry the request to a device
#define ENUMERATION_QUERY_TIMEOUT	3

struct HcEd {                       	/* ----------- HostController EndPoint Descriptor ------------- */
    volatile  uint32_t  Control;		/* Endpoint descriptor control                              */
    volatile  uint32_t  TailTd;			/* Physical address of tail in Transfer descriptor list     */
    volatile  uint32_t  HeadTd;			/* Physcial address of head in Transfer descriptor list     */
    volatile  uint32_t  Next;			/* Physical address of next Endpoint descriptor             */
};

#define TD_CONTROL		1
#define TD_ISOCHRONOUS	2
#define TD_BULK			3
#define TD_INTERRUPT	4
struct HcTd {                       	/* ------------ HostController Transfer Descriptor ------------ */
    volatile  uint32_t  Control;		/* Transfer descriptor control                              */
    volatile  uint32_t  CurrBufPtr;		/* Physical address of current buffer pointer               */
    volatile  uint32_t  Next;			/* Physical pointer to next Transfer Descriptor             */
    volatile  uint32_t  BufEnd;			/* Physical address of end of buffer                        */
    // From here everything is for HCD purpose only
    volatile uint8_t type;
    volatile uint8_t queued;
    volatile uint8_t unused1[2];
    HCDRequest* request;
    HCDEventListener* listener;
    volatile uint32_t unused2[1];
};

#define HCCA_INTERRUPT_NUMBER	32
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
	uint8_t lowSpeed;
	uint8_t deviceAddressesStart;
};

// From NetBSD OHCI
/* Reverse the bits in a value 0 .. 31 */
const uint8_t revbits[32] =
  { 0x00, 0x10, 0x08, 0x18, 0x04, 0x14, 0x0c, 0x1c,
	0x02, 0x12, 0x0a, 0x1a, 0x06, 0x16, 0x0e, 0x1e,
	0x01, 0x11, 0x09, 0x19, 0x05, 0x15, 0x0d, 0x1d,
	0x03, 0x13, 0x0b, 0x1b, 0x07, 0x17, 0x0f, 0x1f };

/**
 * This class is the Host Controller Driver (HCD).
 *
 * For now, it does not support more than one USB device
 * at a time because of how registerDescriptors() function
 * allocate descriptor. It wouldn't be hard to handle multiple
 * devices but since I only have one USB port on my board, I
 * can't test it so I won't bother implementing it.
 */
class HostControllerDriver {
public:
	HostControllerDriver(OHCI_Typedef* ohciRegisters);
	virtual ~HostControllerDriver();

	uint8_t deviceConnected(uint32_t hubPortNumber) { return rootHubPort[hubPortNumber].deviceConnected; }
	uint8_t deviceEnumerated(uint32_t hubPortNumber) { return rootHubPort[hubPortNumber].deviceEnumerated; }
	UsbDevice* enumerateDevice(uint32_t hubPortNumber);
	UsbDevice* periodicTask();

	uint8_t sendRequest(HCDRequest*);
	MemoryPool* getMemoryPool();
	void freeMemoryPool(MemoryPool*);

	void hcInterrupt();
private:
	Hcca* hcca;
	HcTd* headTd;
	HcTd* tailTd;
	HcEd* ctrlEd;
	HcEd* intInEd;
	HcEd* intOutEd;
	uint8_t* tdBuffer;
	uint8_t* ctrlBuffer;

#define TD_IN_POOL	32
	HcTd* tdPool;

#define ED_IN_POOL	32
	HcEd* edPool;

#define MEMORY_POOL_NUMBER	8
#define MEMORY_POOL_SIZE	0x400
	// USB memory pools used by device driver
	MemoryPool usbMemoryPool[MEMORY_POOL_NUMBER];

	// Dummy EDs that make up the interrupt tree.
	HcEd* interruptTreeNode;

	OHCI_Typedef* ohciRegisters;

	volatile RootHubPort rootHubPort[MAX_ROOT_PORTS];

	// Global flags
	volatile uint8_t transferCompleted;

	void init();
	void portReset(uint32_t hubPortNumber);

	// USB control functions
	uint8_t getDescriptor(uint16_t descriptorTypeIndex, uint16_t descriptorLength, uint8_t* receiveBuffer);
	uint8_t setAddress(uint16_t address);
	uint8_t setConfiguration(uint8_t configuration);

	uint8_t controlRead(uint8_t bmRequestType,
			uint8_t bRequest,
			uint16_t wValue,
			uint16_t wIndex,
			uint16_t wLength,
			uint8_t* receiveBuffer);

	uint8_t controlWrite(uint8_t bmRequestType,
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

	uint8_t launchTransaction(HcEd* ed, uint32_t token, uint8_t* transactionBuffer, uint32_t transactionLength);

	void registerEndpoints(UsbDevice*);
	void unregisterEndpoints(UsbDevice*);

	// Debug Function
	void printDescriptors(UsbDevice* device);
};

#endif /* HOSTCONTROLLERDRIVER_H_ */
