
MEMORY
{
    /*RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 192K
    CCMRAM (rw)      : ORIGIN = 0x10000000, LENGTH = 64K
    FLASH (rx)      : ORIGIN = 0x08010000, LENGTH = 2048K*/
    /*RGREMOVE*/
    FLASH (rx)      : ORIGIN = 0x08010000, LENGTH = 2048K
}


/* Explicitly mention label for the program's entry point */
ENTRY(app_main)

SECTIONS
{
    /* The 'text' section contains the main program code. */
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        app_etext = .;        /* define a global symbols at end of code */
    } >FLASH
    
    /* The 'rodata' section contains read-only data,
     * constants, strings, information that won't change. 
     * variables defined with const keyword in C */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);

    } >FLASH
    
    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM : 
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array     :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH
  
    /* PLT section contains code for accessing the dynamically linked functions
     * ie funtions from shared libraries in a postion independent manner */
    .plt :
    {
        . = ALIGN(4);
        *(.plt)
        . = ALIGN(4);
    } >FLASH
    
    /* The 'data' section is space set aside in RAM for
     * things like non-zero global and static variables, which can change. */
   
    .data :
    {
        /* We cannot declare _sidata here, because it will be assigned the 
         * start of RAM (0x20000000), instead of end of FLASH */
        . = ALIGN(4);
        /* Mark start/end locations for the 'data' section. */
        app_sdata = .;
        *(.data*)
        . = ALIGN(4);
    } >FLASH /* Mentioning RAM here, specifies the VMA ie virtual memory address ie address during execution */
    /* The AT defines the load address of the .data section. This is where the section will be loaded */

    /* _sidata will be the location .data section will be stored
     * in the flash, and _sdata will be the location of .data section 
     * in the RAM. We use these two addresses to copy data from _sidata
     * to _sdata*/
    app_sidata = LOADADDR(.data);
 
    /* The global offset table is the table for indirectly accessing the global variables
     * The table contains addresses of the global variables. The text section contains 
     * a address of the GOT base and a offset in it to access the appropriate variables.
     * This is done to access the variables in a postion independent manner. */
    .got :
    {
        . = ALIGN(4);
        app_sgot = .;
        *(.got)
    } > FLASH
    
    /* got.plt section contains entries which is used with the PLT to access the functions 
     * in a position independent manner. */
    .got.plt :
    {
        . = ALIGN(4);
        *(.got.plt)
        app_edata = .;
    } > FLASH
    
    /* The 'bss' section is similar to the 'data' section,
    *  but its space is initialized to all 0s at the
    *  start of the program. */
    .bss :
    {
        . = ALIGN(4);
        /* Also mark the start/end of the BSS section. */
        app_sbss = .;
        app__bss_start__ = app_sbss;
        *(.bss*)
        /* A special notation is needed for common symbols, 
         * because in many object file formats common symbols 
         * do not have a particular input section. The linker 
         * treats common symbols as though they are in an 
         * input section named ‘COMMON’.*/ 
        *(COMMON)
        . = ALIGN(4);
        app_ebss = .;
        app__bss_end__ = app_ebss;

    } >FLASH
    
    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}


