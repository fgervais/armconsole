/*
 * HostControllerDriver.h
 *
 *  Created on: Jan 22, 2010
 *      Author: fgervais
 */

#ifndef HOSTCONTROLLERDRIVER_H_
#define HOSTCONTROLLERDRIVER_H_

#include <stdint.h>

struct HcEd {                       /* ----------- HostController EndPoint Descriptor ------------- */
    volatile  uint32_t  Control;              /* Endpoint descriptor control                              */
    volatile  uint32_t  TailTd;               /* Physical address of tail in Transfer descriptor list     */
    volatile  uint32_t  HeadTd;               /* Physcial address of head in Transfer descriptor list     */
    volatile  uint32_t  Next;                 /* Physical address of next Endpoint descriptor             */
};

struct HcTd {                       /* ------------ HostController Transfer Descriptor ------------ */
    volatile  uint32_t  Control;              /* Transfer descriptor control                              */
    volatile  uint32_t  CurrBufPtr;           /* Physical address of current buffer pointer               */
    volatile  uint32_t  Next;                 /* Physical pointer to next Transfer Descriptor             */
    volatile  uint32_t  BufEnd;               /* Physical address of end of buffer                        */
};

struct Hcca {                       /* ----------- Host Controller Communication Area ------------  */
    volatile  uint32_t  IntTable[32];         /* Interrupt Table                                          */
    volatile  uint32_t  FrameNumber;          /* Frame Number                                             */
    volatile  uint32_t  DoneHead;             /* Done Head                                                */
    volatile  uint32_t  Reserved[116];        /* Reserved for future use                                  */
    volatile  uint32_t  Unknown[4];           /* Unused                                                   */
};

class HostControllerDriver {
public:
	HostControllerDriver();
	virtual ~HostControllerDriver();

private:
	Hcca *hcca;

	void init();
};

#endif /* HOSTCONTROLLERDRIVER_H_ */
