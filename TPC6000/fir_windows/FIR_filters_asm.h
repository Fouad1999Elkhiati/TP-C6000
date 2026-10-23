/*****************************************************************************/
/* FILENAME                                                                  */
/*   FIR_filters_asm.h                                                       */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header File for different Finite Impulse Response filters implemented   */
/*   as assembly language code, where C code version would be too slow.      */
/*                                                                           */
/*   TMS320C6713 version. The coefficients and the samples are 16-bit        */
/*   (short); the dual filters return a 32-bit int (on the C54 this was a    */
/*   32-bit long; on the C6000 a long is 40 bits, so int is used).           */
/*                                                                           */
/*****************************************************************************/

#ifndef FIR_FILTERS_ASM_H
#define FIR_FILTERS_ASM_H

void FIR_filters_asm_initialize(void);

short FIR_filter_asm(const short * filter_constants, short input);

short FIR_filter_asm_2(const short * filter_constants, short input);

int FIR_dual_filter_asm( const short * filter_constants, short input);

int FIR_dual_filter_variable_asm( const short * filter_constants, short input,
                                  short length);

#endif

/******************************************************************************/
/* End of FIR_filters_asm.h                                                   */
/******************************************************************************/
