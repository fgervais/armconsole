/*****************************************************************************
 *   irq.c: Interrupt handler C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/

/* TODO mthomas - volatiles for vect_addr, vect_cntl? */

#include "LPC23xx.h"			/* LPC23XX/24xx Peripheral Registers */
#include "type.h"
#include "irq.h"

/* Initialize the interrupt controller */
/******************************************************************************
** Function name:		init_VIC
**
** Descriptions:		Initialize VIC interrupt controller.
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void init_VIC(void)
{
    DWORD i = 0;
    DWORD *vect_addr, *vect_cntl;

    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    static DWORD isr_vector_table[] = {
    		(DWORD)wdt_irq_handler,
    		(DWORD)swi_irq_handler
    		};

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
		*vect_addr = isr_vector_table[i];
		/* Set to lowest priority */
		*vect_cntl = 0xF;
    }

    vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX);

    *vect_addr = (DWORD)wdt_irq_handler;
    vect_addr += 4;
    *vect_addr = (DWORD)swi_irq_handler;
    return;
}

/******************************************************************************
** Function name:		install_irq
**
** Descriptions:		Install interrupt handler
** parameters:			Interrupt number, interrupt handler address,
**						interrupt priority
** Returned value:		true or false, return false if IntNum is out of range
**
******************************************************************************/
DWORD install_irq( DWORD IntNumber, void *HandlerAddr, DWORD Priority )
{
    DWORD *vect_addr;
    DWORD *vect_cntl;

    VICIntEnClr = 1 << IntNumber;	/* Disable Interrupt */
    if ( IntNumber >= VIC_SIZE )
    {
		return ( FALSE );
    }
    else
    {
		/* find first un-assigned VIC address for the handler */
		vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber*4);
		vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + IntNumber*4);
		*vect_addr = (DWORD)HandlerAddr;	/* set interrupt vector */
		*vect_cntl = Priority;
		VICIntEnable = 1 << IntNumber;	/* Enable Interrupt */
		return( TRUE );
    }
}

void wdt_irq_handler( void ) {

}

void swi_irq_handler( void ) {

}

/******************************************************************************
**                            End Of File
******************************************************************************/
