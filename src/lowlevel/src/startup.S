@/*****************************************************************************
@*   startup.s: startup file for NXP LPC230x Family Microprocessors
@*
@*   Copyright(C) 2006, NXP Semiconductor
@*   All rights reserved.
@*
@*   History
@*   2006.09.01  ver 1.00    Prelimnary version, first Release
@*
@*****************************************************************************/

@ modified by Martin Thomas:
@  - changed comment char to @ for GNU assembler (arm-elf-as)
@    "The line comment character is [...] �@� on the ARM [...]"
@    (GNU-as manual from binutils V 2.17)
@  - ported RV-ASM to GNU-ASM (sections, import/export, equ,
@    :OR:, Labels etc.)
@  - added .data and .bss inits
@  - call main instead of __main (since there is no "RV runtime-init")
@  - call main with bx so it can be thumb-code
@  - added IRQ-Wrapper for easier porting of ISR-codes for IAR, RV etc.
@  - TargetResetInit can be a thumb-function (using BX instead of BL)
@  - moved TargetResetInit call before the stack-inits and use
@    a temporary stack-pointer
@  - route swi-exception to handler implemented in swi_handler.h

@@	PRESERVE8

@/*
@ *  The STARTUP.S code is executed after CPU Reset. This file may be
@ *  translated with the following SET symbols. In uVision these SET
@ *  symbols are entered under Options - ASM - Define.
@ *
@ *  REMAP: when set the startup code initializes the register MEMMAP
@ *  which overwrites the settings of the CPU configuration pins. The
@ *  startup and interrupt vectors are remapped from:
@ *     0x00000000  default setting (not remapped)
@ *     0x40000000  when RAM_MODE is used
@ *
@ *  RAM_MODE: when set the device is configured for code execution
@ *  from on-chip RAM starting at address 0x40000000.
@ */


@ Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

.equ Mode_USR,            0x10
.equ Mode_FIQ,            0x11
.equ Mode_IRQ,            0x12
.equ Mode_SVC,            0x13
.equ Mode_ABT,            0x17
.equ Mode_UND,            0x1B
.equ Mode_SYS,            0x1F

.equ I_Bit,               0x80            @ when I bit is set, IRQ is disabled
.equ F_Bit,               0x40            @ when F bit is set, FIQ is disabled


@// <h> Stack Configuration (Stack Sizes in Bytes)
@//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:8>
@//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:8>
@//   <o2> Abort Mode          <0x0-0xFFFFFFFF:8>
@//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:8>
@//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:8>
@//   <o5> User/System Mode    <0x0-0xFFFFFFFF:8>
@// </h>

.equ UND_Stack_Size,     0x00000000
.equ SVC_Stack_Size,     0x00000100
.equ ABT_Stack_Size,     0x00000000
.equ FIQ_Stack_Size,     0x00000000
.equ IRQ_Stack_Size,     0x00000100
.equ USR_Stack_Size,     0x00000200

.equ Stack_Size,        (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + \
                         FIQ_Stack_Size + IRQ_Stack_Size + USR_Stack_Size)

@@                AREA    STACK, NOINIT, READWRITE, ALIGN=3
@@
@@Stack_Mem       SPACE   Stack_Size
@@Stack_Top       EQU     Stack_Mem + Stack_Size

.arm
.section .stack, "w"
.align 3
Stack_Mem:
.space Stack_Size
@.equ Stack_Top, Stack_Mem + Stack_Size
@.equ Stack_Top,	__stack


@// <h> Heap Configuration
@//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
@// </h>

@@Heap_Size       EQU     0x00000000
@@                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
@@Heap_Mem        SPACE   Heap_Size

.equ Heap_Size,   0x00000000
.section .heap, "w"
.align 3
HeapMem:
.if (Heap_Size>0)
.space Heap_Size
.endif


@ Area Definition and Entry Point
@  Startup Code must be linked first at Address at which it expects to run.

@@                AREA    RESET, CODE, READONLY
@@                ARM
.section .reset, "ax"
.arm

@ Exception Vectors
@  Mapped to Address 0.
@  Absolute addressing mode must be used.
@  Dummy Handlers are implemented as infinite loops which can be modified.

Vectors:        LDR     PC, Reset_Addr
                LDR     PC, Undef_Addr
                LDR     PC, SWI_Addr
                LDR     PC, PAbt_Addr
                LDR     PC, DAbt_Addr
                NOP                            @ Reserved Vector
                LDR     PC, IRQ_Addr
@@                LDR     PC, [PC, #-0x0120]     @ Vector from VicVectAddr
                LDR     PC, FIQ_Addr

Reset_Addr:      .word     Reset_Handler
Undef_Addr:      .word     Undef_Handler
SWI_Addr:        .word     SWI_Handler
PAbt_Addr:       .word     PAbt_Handler
DAbt_Addr:       .word     DAbt_Handler
                 .word     0xB9206E28             @ Reserved Address
IRQ_Addr:        .word     IRQ_Handler
FIQ_Addr:        .word     FIQ_Handler

Undef_Handler:   B       Undef_Handler
@@ SWI_Handler:     B       SWI_Handler
.extern SoftwareInterrupt
SWI_Handler:     B       SoftwareInterrupt  @ see swi_handler.S
PAbt_Handler:    B       PAbt_Handler
DAbt_Handler:    B       DAbt_Handler
@@ handled thru assembler wrapper (see below)
@@ IRQ_Handler:    B       IRQ_Handler
FIQ_Handler:     B       FIQ_Handler


@ Reset Handler
@                EXPORT  Reset_Handler
.global Reset_handler
Reset_Handler:

@  Call low-level init C-function
@                IMPORT TargetResetInit
@                BL   TargetResetInit

                 .extern TargetResetInit
				 LDR     SP, =__preinit_stack    @ temporary stack at Stack_Top
                 LDR R0, =TargetResetInit
                 MOV LR, PC
                 BX R0


@  Setup Stack for each mode
				LDR     R0, =__postinit_stack

@  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND | I_Bit | F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

@  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT | I_Bit | F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

@  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ | I_Bit | F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

@  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ | I_Bit | F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

@  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC | I_Bit | F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

@  Enter User Mode and set its Stack Pointer
@  mt:          MSR     CPSR_c, #Mode_USR
                MSR     CPSR_c, #Mode_USR | I_Bit
                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size
@  mt: Start application in USR-mode with IRQ-exceptions disabled.
@  They can be enabled at runtime thru IntEnable in swi.h/swi_handler.S.

@  Relocate .data section (Copy from ROM to RAM)
                LDR     R1, =_etext
                LDR     R2, =_sdata
                LDR     R3, =_edata
                CMP     R2, R3
                BEQ     DataIsEmpty
LoopRel:        CMP     R2, R3
                LDRLO   R0, [R1], #4
                STRLO   R0, [R2], #4
                BLO     LoopRel
DataIsEmpty:

@  Clear .bss section (Zero init)
                MOV     R0, #0
                LDR     R1, =_sbss
                LDR     R2, =_ebss
                CMP     R1,R2
                BEQ     BSSIsEmpty
LoopZI:         CMP     R1, R2
                STRLO   R0, [R1], #4
                BLO     LoopZI
BSSIsEmpty:


@  Enter the C code
@                IMPORT  __main
@                LDR     R0, =__main
				.extern __libc_init_array
				LDR R0, =__libc_init_array
				MOV LR, PC
				BX      R0
                .extern main
                LDR R0, =main
                BX      R0


@ User Initial Stack & Heap (not used in GNU port)
@@                AREA    |.text|, CODE, READONLY

@@                IMPORT  __use_two_region_memory
@@                EXPORT  __user_initial_stackheap
@@__user_initial_stackheap

@@                LDR     R0, =  Heap_Mem
@@                LDR     R1, =(Stack_Mem + USR_Stack_Size)
@@                LDR     R2, = (Heap_Mem +      Heap_Size)
@@                LDR     R3, = Stack_Mem
@@                BX      LR


@@ IRQ_Wrapper based on Examples for
@@ AT91-ARM7TDMI AIC from Atmel,
@@ adapted to LPC23xx/24xx VIC by M. Thomas
@@ This wrapper avoids compiler-dependencies.

.set LPC_BASE_VIC, 0xFFFFF000
.set VIC_VectAddr, 0xF00

.arm
IRQ_Handler:
@- Manage Exception Entry
@- Adjust and save LR_irq in IRQ stack
            sub         lr, lr, #4
            stmfd       sp!, {lr}

@- Save SPSR need to be saved for nested interrupt
            mrs         r14, SPSR
            stmfd       sp!, {r14}

@- Save and r0 in IRQ stack
            stmfd       sp!, {r0}
			
@- Load the ISR-Address from VICVectAddr
            ldr         r14, =LPC_BASE_VIC
            ldr         r0 , [r14, #VIC_VectAddr]

@- Enable Interrupt and Switch in Supervisor Mode
           msr         CPSR_c, #Mode_SVC

@- Save scratch/used registers and LR in User Stack
            stmfd       sp!, { r1-r3, r12, r14 }
			
@- Branch to the routine pointed by the VIC_VectAddr
            mov         r14, pc
            bx          r0
			
@- Restore scratch/used registers and LR from User Stack
            ldmia       sp!, { r1-r3, r12, r14 }

@- Disable Interrupt and switch back in IRQ mode
            msr         CPSR_c, #I_Bit | Mode_IRQ

@-  Mark the End of Interrupt on the VIC
@   by writing to VICVectAddr - not needed
@   here since already done in the ISRs
@@           ldr         r14, =LPC_BASE_VIC
@@           str         r14, [r14, #VIC_VectAddr]

			ldr         r14, =LPC_BASE_VIC
			str         r14, [r14, #VIC_VectAddr]

@- Restore R0
            ldmia       sp!, {r0}

@- Restore SPSR_irq and r0 from IRQ stack
            ldmia       sp!, {r14}
            msr         SPSR_cxsf, r14

@- Restore adjusted  LR_irq from IRQ stack directly in the PC
            ldmia       sp!, {pc}^


@                END
.end

