/*****************************************************************************/
/* firmacd.cmd - Linker command file, TMS320C6713 DSK, assembly-only program */
/*                                                                           */
/* The C6000 has a single address space: code (.text), coefficients (.coef) */
/* and data (.bss) are all in the internal RAM. On the C5416 they were in    */
/* PAGE 0 (program) and PAGE 1 (data).                                       */
/* Entry point: _start (linker option -e _start, set in the project).        */
/*****************************************************************************/

MEMORY
{
    PRAM  : origin = 0x00000000, length = 0x00010000   /* internal RAM, code   */
    INRAM : origin = 0x00010000, length = 0x00020000   /* internal RAM, data   */
}

SECTIONS
{
    .text  >  PRAM
    .coef  >  INRAM
    .data  >  INRAM
    .bss   >  INRAM
}
