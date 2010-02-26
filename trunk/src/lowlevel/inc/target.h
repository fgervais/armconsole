/*****************************************************************************
 *   target.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TARGET_H
#define __TARGET_H

#ifdef __cplusplus
   extern "C" {
#endif

/* CPU frequencies and PLL dividers */
#define Fosc				16000000
#define PLL_MValue			9
#define PLL_NValue			1
#define CCLKDivValue		4
#define USBCLKDivValue		6
#define LCDCLKDivValue		8
#define Fcclk				72000000
#define Fcco				288000000
/* APB clock frequence , must be 1/2/4 multiples of ( Fcclk/4 ). */
/* If USB is enabled, the minimum APB must be greater than 16Mhz */
#define Fpclk				(Fcclk / 1)

extern void TargetInit(void);
extern void ConfigurePLL(void);
extern void TargetResetInit(void);
extern void ConfigureSDRAM(void);

#ifdef __cplusplus
   }
#endif

#endif /* end __TARGET_H */
/******************************************************************************
**                            End Of File
******************************************************************************/
