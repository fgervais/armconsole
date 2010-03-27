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
#include "LPC2478.h"
#include "HostControllerDriver.h"
#include "Debug.h"

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
    //DWORD *vect_cntl;
    DWORD *vect_addr;

    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    static DWORD isr_vector_table[VIC_SIZE] = {
    		(DWORD)wdt_irq_handler,
    		(DWORD)swi_irq_handler,
    		(DWORD)armcore0_irq_handler,
    		(DWORD)armcore1_irq_handler,
    		(DWORD)timer0_irq_handler,
    		(DWORD)timer1_irq_handler,
    		(DWORD)uart0_irq_handler,
    		(DWORD)uart1_irq_handler,
    		(DWORD)pwm0_1_irq_handler,
    		(DWORD)i2c0_irq_handler,
    		(DWORD)spi0_irq_handler,
    		(DWORD)spi1_irq_handler,
    		(DWORD)pll_irq_handler,
    		(DWORD)rtc_irq_handler,
    		(DWORD)eint0_irq_handler,
    		(DWORD)eint1_irq_handler,
    		(DWORD)eint2_irq_handler,
    		(DWORD)eint3_irq_handler,
    		(DWORD)adc0_irq_handler,
    		(DWORD)i2c1_irq_handler,
    		(DWORD)bod_irq_handler,
    		(DWORD)emac_irq_handler,
    		(DWORD)usb_irq_handler,
    		(DWORD)can_irq_handler,
    		(DWORD)mci_irq_handler,
    		(DWORD)gpdma_irq_handler,
    		(DWORD)timer2_irq_handler,
    		(DWORD)timer3_irq_handler,
    		(DWORD)uart2_irq_handler,
    		(DWORD)uart3_irq_handler,
    		(DWORD)i2c2_irq_handler,
    		(DWORD)i2s_irq_handler
    		};

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		//vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
		*vect_addr = isr_vector_table[i];
		/* Set to lowest priority */
		//*vect_cntl = 0xF;
    }
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

void armcore0_irq_handler( void ) {

}

void armcore1_irq_handler( void ) {

}

extern void disk_timerproc();
void timer0_irq_handler( void ) {
	T0IR |= 1;

	// sd_spi_lpc200.c
	disk_timerproc();
}

void timer1_irq_handler( void ) {

}

void uart0_irq_handler( void ) {

}

void uart1_irq_handler( void ) {

}

void pwm0_1_irq_handler( void ) {

}

void i2c0_irq_handler( void ) {

}

void spi0_irq_handler( void ) {

}

void spi1_irq_handler( void ) {

}

void pll_irq_handler( void ) {

}

void rtc_irq_handler( void ) {

}

void eint0_irq_handler( void ) {

}

void eint1_irq_handler( void ) {

}

void eint2_irq_handler( void ) {

}

void eint3_irq_handler( void ) {

}

void adc0_irq_handler( void ) {

}

void i2c1_irq_handler( void ) {

}

void bod_irq_handler( void ) {

}

void emac_irq_handler( void ) {

}

void usb_irq_handler( void ) {
	LPC2478::getHCD()->hcInterrupt();
}

void can_irq_handler( void ) {

}

void mci_irq_handler( void ) {

}

void gpdma_irq_handler( void ) {

}

void timer2_irq_handler( void ) {

}

void timer3_irq_handler( void ) {

}

void uart2_irq_handler( void ) {

}

void uart3_irq_handler( void ) {

}

void i2c2_irq_handler( void ) {

}

void i2s_irq_handler( void ) {

}

/******************************************************************************
**                            End Of File
******************************************************************************/
