
MEMORY
{
    /* TODO: Change the LENGTH to a resonable value */
    FLASH (rwx)      : ORIGIN = 0x00000000, LENGTH = 2048K
    RAM(rwx)         : ORIGIN = 0x10000000, LENGTH = 32K
}


/* Explicitly mention label for the program's entry point */
ENTRY(AppStart)

SECTIONS
{
    /* The 'text' section contains the main program code. */
    .text :
    {
        *(.AppStart)
        *(.text.main)
        *(.text*)
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))
        /* The 'rodata' section contains read-only data,
        * constants, strings, information that won't change. 
        * variables defined with const keyword in C */
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
        . = ALIGN(4);
        /* Mark start/end locations for the 'data' section. */
        app_sdata = .;
        *(.sys_struct)
        *(.data*)
        . = ALIGN(4);
    } >RAM
    app_sidata = LOADADDR(.data);
  
    .got :
    {
        . = ALIGN(4);
        app_sgot = .;
        *(.got)        
        *(.got.plt)
        app_edata = .;     
    } > RAM
    
    /* The 'bss' section is similar to the 'data' section,
    *  but its space is initialized to all 0s at the
    *  start of the program. */
    .bss :
    {
        . = ALIGN(4);
        /* Also mark the start/end of the BSS section. */
        app_sbss = .;
        *(.bss*)
        /* A special notation is needed for common symbols, 
         * because in many object file formats common symbols 
         * do not have a particular input section. The linker 
         * treats common symbols as though they are in an 
         * input section named ‘COMMON’.*/ 
        *(COMMON)
        . = ALIGN(4);
        app_ebss = .;

    } >RAM
    
    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}


