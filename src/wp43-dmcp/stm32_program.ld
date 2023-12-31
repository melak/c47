/******************************************************************************************************
 *  File        : stm32_flash.ld
 *  Abstract    : Linker script for STM32L476ZG Device with 1024KByte FLASH, 96KByte RAM
 *                Set heap size, stack size and stack location according to application requirements.
 *                Set memory bank area and size if external memory is used.
 *  Target      : STMicroelectronics STM32
 *  Distribution: The file is distributed as is, without any warranty of any kind.
 *  Swissmicros - ld script for STM32 + QSPI
 *****************************************************************************************************/

/* Specify the memory areas */
MEMORY {
  /* FLASH   (rx) : ORIGIN = 0x08000000, LENGTH = 1024K     */
  /* RAM    (xrw) : ORIGIN = 0x20000000, LENGTH =   96K     */
  /* RAM0   (xrw) : ORIGIN = 0x10000000, LENGTH =   32K-256 */
  FLASH   (rx) : ORIGIN = 0x08050000, LENGTH =  704K
  RAM    (xrw) : ORIGIN = 0x10000000, LENGTH =    8K
  QSPI    (rx) : ORIGIN = 0x90000000, LENGTH = 2048K-12K /* a 4K block from 0x901FD000 to 0x901FDFFF is reserved by DMCP */
}

/* Entry Point */
ENTRY(Program_Entry)

/* Define output sections */
SECTIONS {
  .qspi : {
    . = ALIGN(8);
    _qspi_start = .;   /* create a global symbol at qspi start */
    *(.qspi)           /* .qspi  sections */
    *(.rodata)         /* Added by Martin .rodata  sections (constants, strings, etc.) */
    *(.rodata*str1.1)  /* Added by Martin .rodata  sections (constants, strings, etc.) */

    . = ALIGN(8);
    _qspi_end = .;     /* define a global symbols at end of qspi */
  } >QSPI /* AT> FLASH */

  /* Constant data goes into FLASH */
  .rodata : {
    KEEP(*(.rodata.prog_info)) /* Program info */
    . = ALIGN(64);
    *(.rodata)         /* .rodata  sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(8);
  } >FLASH

  _qspi_init_base = LOADADDR(.qspi);
  _qspi_init_length = SIZEOF(.qspi);

  /* The program code and other data goes into FLASH */
  .text : {
    . = ALIGN(8);
    *(.text)           /* .text  sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(8);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  .ARM.extab   : {
    . = ALIGN(8);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(8);
  } >FLASH

  .ARM : {
    . = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(8);
  } >FLASH

  .preinit_array     : {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(8);
  } >FLASH

  .init_array : {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(8);
  } >FLASH

  .fini_array : {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(8);
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : {
    . = ALIGN(8);
    _sdata = .;        /* create a global symbol at data start */
    *(.data.sdb)       /* SDB at the beginning of the RAM0 */
    *(.data)           /* .data  sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(8);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss : {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM AT> RAM

  /* Remove information from the standard libraries */
  /DISCARD/ : {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
    libgmp.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
