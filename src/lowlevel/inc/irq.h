/******************************************************************************
 *   irq.h:  Interrupt related Header file for NXP LPC23xx/24xx Family
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __IRQ_H
#define __IRQ_H

#include "type.h"

#ifdef __cplusplus
   extern "C" {
#endif

#define I_Bit			0x80
#define F_Bit			0x40

#define SYS32Mode		0x1F
#define IRQ32Mode		0x12
#define FIQ32Mode		0x11

#define HIGHEST_PRIORITY	0x01
#define LOWEST_PRIORITY		0x0F

#define	WDT_INT			0
#define SWI_INT			1
#define ARM_CORE0_INT		2
#define	ARM_CORE1_INT		3
#define	TIMER0_INT		4
#define TIMER1_INT		5
#define UART0_INT		6
#define	UART1_INT		7
#define	PWM0_1_INT		8
#define I2C0_INT		9
#define SPI0_INT		10			/* SPI and SSP0 share VIC slot */
#define SSP0_INT		10
#define	SSP1_INT		11
#define	PLL_INT			12
#define RTC_INT			13
#define EINT0_INT		14
#define EINT1_INT		15
#define EINT2_INT		16
#define EINT3_INT		17
#define	ADC0_INT		18
#define I2C1_INT		19
#define BOD_INT			20
#define EMAC_INT		21
#define USB_INT			22
#define CAN_INT			23
#define MCI_INT			24
#define GPDMA_INT		25
#define TIMER2_INT		26
#define TIMER3_INT		27
#define UART2_INT		28
#define UART3_INT		29
#define I2C2_INT		30
#define I2S_INT			31

#define VIC_SIZE		32

#define VECT_ADDR_INDEX	0x100
#define VECT_CNTL_INDEX 0x200

/* Be aware that, from compiler to compiler, nested interrupt will have to
be handled differently. More details can be found in Philips LPC2000
family app-note AN10381 */


// mthomas
#ifdef __GNUC__
#define __irq
#define IENABLE
#define IDISABLE
#else
/* unlike Keil CARM Compiler, in ARM's RealView compiler, don't save and
restore registers into the stack in RVD as the compiler does that for you.
See RVD ARM compiler Inline and embedded assemblers, "Rules for
using __asm and asm keywords. */
static DWORD sysreg;		/* used as LR register */
#define IENABLE __asm { MRS sysreg, SPSR; MSR CPSR_c, #SYS32Mode }
#define IDISABLE __asm { MSR CPSR_c, #(IRQ32Mode|I_Bit); MSR SPSR_cxsf, sysreg }
#endif

void init_VIC( void );
DWORD install_irq( DWORD IntNumber, void *HandlerAddr, DWORD Priority );

/* Every possible interrupt handler */
void wdt_irq_handler( void );
void swi_irq_handler( void );
void armcore0_irq_handler( void );
void armcore1_irq_handler( void );
void timer0_irq_handler( void );
void timer1_irq_handler( void );
void uart0_irq_handler( void );
void uart1_irq_handler( void );
void pwm0_1_irq_handler( void );
void i2c0_irq_handler( void );
void spi0_irq_handler( void );
void spi1_irq_handler( void );
void pll_irq_handler( void );
void rtc_irq_handler( void );
void eint0_irq_handler( void );
void eint1_irq_handler( void );
void eint2_irq_handler( void );
void eint3_irq_handler( void );
void adc0_irq_handler( void );
void i2c1_irq_handler( void );
void bod_irq_handler( void );
void emac_irq_handler( void );
void usb_irq_handler( void );
void can_irq_handler( void );
void mci_irq_handler( void );
void gpdma_irq_handler( void );
void timer2_irq_handler( void );
void timer3_irq_handler( void );
void uart2_irq_handler( void );
void uart3_irq_handler( void );
void i2c2_irq_handler( void );
void i2s_irq_handler( void );

#ifdef __cplusplus
   }
#endif

#endif /* end __IRQ_H */

/******************************************************************************
**                            End Of File
******************************************************************************/
