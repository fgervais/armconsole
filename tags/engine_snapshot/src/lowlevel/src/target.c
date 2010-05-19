/*****************************************************************************
 *   target.c:  Target C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"

/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary
**						place, change it as needed
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TargetInit(void)
{
    /* Add your codes here */
    return;
}

/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure PLL switching to main OSC instead of IRC
**						at power up and wake up from power down.
**						This routine is used in TargetResetInit() and those
**						examples using power down and wake up such as
**						USB suspend to resume, ethernet WOL, and power management
**						example
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void ConfigurePLL ( void )
{
	DWORD MValue, NValue;

	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;			/* Enable PLL, disconnected */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
    }

    PLLCON = 0;				/* Disable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

	SCS |= 0x20;			/* Enable main OSC */
    while( !(SCS & 0x40) );	/* Wait until main OSC is usable */

    CLKSRCSEL = 0x1;		/* select main OSC as the PLL clock source */

    PLLCFG = (PLL_MValue-1) | ((PLL_NValue-1) << 16);
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

    PLLCON = 1;				/* Enable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

    CCLKCFG = CCLKDivValue-1;		/* Set clock divider */
    USBCLKCFG = USBCLKDivValue-1;	/* usbclk = 288 MHz/6 = 48 MHz */
    LCD_CFG = LCDCLKDivValue-1;		/* LCD panel clock = 72 MHz/8 = 9 MHz */

    while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */

    MValue = (PLLSTAT & 0x00007FFF) + 1;
    NValue = ((PLLSTAT & 0x00FF0000) >> 16) + 1;
    while ((MValue != PLL_MValue) && ( NValue != PLL_NValue) );

    PLLCON = 3;				/* enable and connect */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
	while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */
	return;
}

/******************************************************************************
** Function name:		TargetResetInit
**
** Descriptions:		Initialize the target board before running the main()
**						function; User may change it as needed, but may not
**						deleted it.
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TargetResetInit(void)
{
#ifdef __DEBUG_RAM
    MEMMAP = 0x2;			/* remap to internal RAM */
#else
    MEMMAP = 0x1;			/* remap to internal flash */
#endif

    /* Configure PLL, switch from IRC to Main OSC */
	ConfigurePLL();

  /* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
    PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
    PCLKSEL1 = 0x00000000;
#elif (Fpclk / (Fcclk / 4)) == 2
    PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
    PCLKSEL1 = 0xAAAAAAAA;
#elif (Fpclk / (Fcclk / 4)) == 4
    PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
    PCLKSEL1 = 0x55555555;
#endif

    /* Set memory accelerater module*/
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#elif Fcclk < 40000000
    MAMTIM = 2;
#elif Fcclk < 60000000
    MAMTIM = 3;
#else
    MAMTIM = 4;
#endif
    MAMCR = 2;

	init_VIC();

	ConfigureSDRAM();

    return;
}

void ConfigureSDRAM(void) {
	// Code below are modified from AN10771 from NXP
	DWORD wtemp;
	volatile int i;

	// Start EMC clock
	PCONP |= (1 << 11);

	PINSEL6 |= 0x55555555;		// D0-D15
	PINSEL7 |= 0x55555555;		// D16-D31
	PINSEL8 |= 0x15555555;		// A0-A14
	PINSEL9 |= (1 << 18);		// WE
	PINSEL5 |= (1 << 0);		// CAS
	PINSEL5 |= (1 << 2);		// RAS
	PINSEL5 |= (1 << 4);		// CLKOUT0
	PINSEL5 |= (1 << 8);		// DYCS0
	PINSEL5 |= (1 << 16);		// CKEOUT0
	PINSEL5 |= (1 << 24);		// DQMOUT0
	PINSEL5 |= (1 << 26);		// DQMOUT1
	PINSEL5 |= (1 << 28);		// DQMOUT2
	PINSEL5 |= (1 << 30);		// DQMOUT3

	EMC_CTRL = 1;				// Enable EMC

	EMC_DYN_RD_CFG = 1;			// Configures the dynamic memory read strategy(Command delayed strategy)

	/*
	 * NOTE : All these are calculated using 72 MHz clock
	 * i.e. 13.8889 ns period
	 */

	/*
	 * CAS latency = 2
	 * RAS latency(active to read/write delay) = 2
	 */
	EMC_DYN_RASCAS0 = 0x202;

	/*
	 * tRP = 20 ns
	 * 1 + 1 clock cycles
	 */
	EMC_DYN_RP = 1;

	/*
	 * tRAS = 44 ns
	 * 3 + 1 clock cycles
	 */
	EMC_DYN_RAS = 3;

	/*
	 * From the datasheet :
	 * This value is normally found in SDRAM data sheets as tSREX, for devices
	 * without this parameter you use the same value as tXSR.
	 *
	 * tXSR = 75 ns
	 * 5 + 1 clock cycles
	 */
	EMC_DYN_SREX = 5;

	/*
	 * 2 + 1 clock cycle. (From BlueScreen SUN demo)
	 */
	EMC_DYN_APR = 2;

	/*
	 * 4 + 1 clock cycle. (From BlueScreen SUN demo)
	 */
	EMC_DYN_DAL = 4;

	/*
	 * 1 CK + 7.5 ns
	 * 1 + 1 clock cycle
	 */
	EMC_DYN_WR = 1;

	/*
	 * 66 ns
	 * 4 + 1 clock cyles
	 */
	EMC_DYN_RC = 4;
	EMC_DYN_RFC = 4;

	/*
	 * tXSR = 75 ns
	 * 5 + 1 clock cycles
	 */
	EMC_DYN_XSR = 5;

	/*
	 * 15 CK
	 * 14 + 1 clock cyclces
	 */
	EMC_DYN_RRD = 14;

	/*
	 * 2 + 1 clock cycle. (From BlueScreen SUN demo)
	 */
	EMC_DYN_MRD = 2;

	/* 256 MB (16Mx16), 4 banks, row length = 13, column length = 9 */
	//EMC_DYN_CFG0 = 0x00000480;
	/* 256 MB (16Mx16), 4 banks, row length = 13, column length = 9 */
	EMC_DYN_CFG0 = 0x00004680;

	// JEDEC General SDRAM Initialization Sequence
	// DELAY to allow power and clocks to stabilize ~100 us
	// NOP
	EMC_DYN_CTRL = 0x0183;
	//Issue SDRAM NOP (no operation) command ; CLKOUT runs continuously;All clock enables are driven HIGH continuously

	for(i = 200*30; i>0; i--);

	EMC_DYN_CTRL = 0x00000103;	//Issue SDRAM PALL (precharge all) command.
	EMC_DYN_RFSH = 1;			//Indicates 1X16 CCLKs between SDRAM refresh cycles.
	for(i = 128; i>0; --i);		// > 128 clk

	/*
	 * Indicates 35*16 CCLKs between SDRAM refresh cycles.
	 * 64 ms, 8192-cycle
	 */
	EMC_DYN_RFSH = 35;

	EMC_DYN_CTRL = 0x00000083;	//Issue SDRAM MODE command.

	/* Set mode register in SDRAM */
		/* Mode regitster table for Micron's MT48LCxx */
		/* bit 9:   Programmed burst length(0)
		 bit 8~7: Normal mode(0)
		 bit 6~4: CAS latency 3
		 bit 3:   Sequential(0)
		 bit 2~0: Burst length is 4 for 32-bit bus
		 row position is 13 */
		//dummy = *((volatile unsigned short *)(SDRAM_BASE_ADDR | (0x23 << 12)));
	/*
	 * Set mode register in SDRAM
	 * Mode regitster table for Micron's MT48LCxx
	 * bit 9:   Programmed burst length(0)
	 * bit 8~7: Normal mode(0)
	 * bit 6~4: CAS latency 2
	 * bit 3:   Sequential(0)
	 * bit 2~0: Burst length is 4 for 32-bit bus
	 * row position is 13
	 */
	wtemp = *((volatile unsigned long *)(DYNAMIC_MEM0_BASE | (0x22 << 13)));

	EMC_DYN_CTRL = 0x0000;		//Issue SDRAM norm command ; CLKOUT stop;All clock enables low

	EMC_DYN_CFG0 |= 0x80000;	//Buffer enabled for accesses to DCS0 chip


}

/******************************************************************************
**                            End Of File
******************************************************************************/
