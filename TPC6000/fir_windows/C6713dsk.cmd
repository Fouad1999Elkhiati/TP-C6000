/*****************************************************************************/
/* C6713dsk.cmd - Linker command file for the TMS320C6713 DSK                */
/*                (replaces the C5416 DSP/BIOS generated xxxcfg.cmd)         */
/*                                                                           */
/* Everything is placed in the 192 KB internal RAM of the C6713.             */
/* The C6000 has a single address space (no PAGE 0 / PAGE 1 as on the C54). */
/*****************************************************************************/

-stack 0x800
-heap  0x800

MEMORY
{
    IRAM     : origin = 0x00000000,  length = 0x00030000   /* internal RAM  */
    SDRAM    : origin = 0x80000000,  length = 0x01000000   /* 16 MB SDRAM   */
    FLASH    : origin = 0x90000000,  length = 0x00040000   /* 256 KB flash  */
}

SECTIONS
{
    .vectors  >  IRAM
    .text     >  IRAM       /* code                                         */
    .stack    >  IRAM       /* C stack                                      */
    .bss      >  IRAM       /* global and static variables, asm buffers     */
    .cinit    >  IRAM       /* initial values of C variables                */
    .pinit    >  IRAM
    .cio      >  IRAM       /* C I/O buffer used by puts()                  */
    .const    >  IRAM       /* const data: the filter coefficients          */
    .data     >  IRAM
    .switch   >  IRAM
    .sysmem   >  IRAM       /* heap                                         */
    .far      >  IRAM
    .csldata  >  IRAM       /* Chip Support Library data                    */
    .fardata  >  IRAM       /* ELF (EABI) sections, used by recent compilers */
    .neardata >  IRAM
    .rodata   >  IRAM
    .init_array > IRAM
    .args     >  IRAM
}
