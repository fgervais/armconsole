/*
Default linker script for STM32F10x_512K_64K
Copyright RAISONANCE S.A.S. 2008

Modified for High Density devices
by Francois Gervais, August 2009
Modified for C++ support with the help of generic.ld
from CodeSourcery G++ Lite
by Francois Gervais, Septembre 2009
Modified for LPC2400 devices
byt Francois Gervais, December 2009
*/

/* include the memory spaces definitions sub-script */

/* Memory Spaces Definitions */

MEMORY
{
  ON_CHIP_FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
  ON_CHIP_RAM (xrw) : ORIGIN = 0x40000000, LENGTH = 64K
  /* The video memory takes the first 1 MB of SDRAM */
  OFF_CHIP_SDRAM (xrw) : ORIGIN = 0xA0100000, LENGTH = 63M
}

/* higher address of the user mode stack */
__preinit_stack = 0x40010000;
__postinit_stack = 0xA4000000;



/* include the sections management sub-script for FLASH mode */

/* Sections Definitions */

SECTIONS
{
    /* Section defined in startup.S file */
    .reset :
    {
		. = ALIGN(4);
        KEEP(*(.reset))            /* Startup code */
		. = ALIGN(4);
    } >ON_CHIP_FLASH
    
    /* Section defined in swi_handler.S file */
    .swi_handler :
	{
		. = ALIGN(4);
		KEEP(*(.swi_handler))
		. = ALIGN(4);
	} >ON_CHIP_FLASH
    
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
	    . = ALIGN(4);
        
        *(.text .text.* .gnu.linkonce.t.*)
    	*(.plt)
    	*(.gnu.warning)
    	*(.glue_7t) *(.glue_7) *(.vfp11_veneer)

    	*(.ARM.extab* .gnu.linkonce.armextab.*)
    	*(.gcc_except_table)
	   
    } >ON_CHIP_FLASH
    
    
    
    .eh_frame_hdr : ALIGN (4)
	{
		KEEP (*(.eh_frame_hdr))
	} >ON_CHIP_FLASH
	.eh_frame : ALIGN (4)
	{
		KEEP (*(.eh_frame))
	} >ON_CHIP_FLASH
	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >ON_CHIP_FLASH
    __exidx_end = .;
    
    .rodata : ALIGN (4)
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	
		. = ALIGN(4);
		KEEP(*(.init))
	
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
	
		. = ALIGN(4);
		KEEP(*(.fini))
	
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
	
		. = ALIGN(4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
	
		. = ALIGN(4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))
	
		. = ALIGN(4);
		_etext = .;
	    /* This is used by the startup in order to initialize the .data secion */
		_sidata = _etext;
	} >ON_CHIP_FLASH
    
    

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data secion */
        _sdata = . ;
        
        KEEP(*(.jcr))
		*(.got.plt) *(.got)
		*(.shdata)
		*(.data .data.* .gnu.linkonce.d.*)

	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .data secion */
   	 	_edata = . ;
	} >OFF_CHIP_SDRAM
    
    

    /* This is the uninitialized data section */
    .bss :
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;
        
        *(.shbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
        
	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .bss secion */
   	 	_ebss = . ;
	} >OFF_CHIP_SDRAM
    
    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    
    /* Heap and Stack are dummy sections that can be removed.
	They were both in the original file so I let them there. With the stack
	section, if you have a bigger stack than the size of you memory, the linker
	should tell you. Keep in mind that what is important is the definition of
	__stack on top of that file. */
    
    .heap :
	{
		. = ALIGN(4);
		_sheap = .;
		
		*(.HEAP)
		
		. = ALIGN(4);
		_eheap = .;
	} >OFF_CHIP_SDRAM
    
    .stack ALIGN(256) :
	{
		. = ALIGN(4);
		_sstack = .;
		
		*(.stack)
		
		_estack = .;
	} >OFF_CHIP_SDRAM
    
    
    /* after that it's only debugging information. */
    
    /* remove the debugging information from the standard libraries */
    DISCARD :
    {
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
    
    note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
	.ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
	/DISCARD/ : { *(.note.GNU-stack) }
}
