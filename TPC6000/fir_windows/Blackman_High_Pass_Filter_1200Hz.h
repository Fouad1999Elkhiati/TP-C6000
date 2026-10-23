/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	blackman_high_pass_filter_1200Hz.h                                       */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file FIR highpass filter with 1200 Hz cutoff using a             */
/*   sampling frequency of 48000 Hz and a Blackman window. 51 coefficients.  */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora.                           */
/*                                                                           */
/*****************************************************************************/

const short blackman_high_pass_filter_1200Hz[] = {

           0x0000, /* H0 */
           0x0000, /* H1 */
           0x0001, /* H2 */
           0x0002, /* H3 */
           0x0002, /* H4 */
           0x0000, /* H5 */
           0xFFFB, /* H6 */
           0xFFF1, /* H7 */
           0xFFDF, /* H8 */
           0xFFC4, /* H9 */
           0xFF9D, /* H10 */
           0xFF68, /* H11 */
           0xFF23, /* H12 */
           0xFECC, /* H13 */
           0xFE65, /* H14 */
           0xFDEC, /* H15 */
           0xFD66, /* H16 */
           0xFCD5, /* H17 */
           0xFC3F, /* H18 */
           0xFBAA, /* H19 */
           0xFB1B, /* H20 */
           0xFA9B, /* H21 */
           0xFA2F, /* H22 */
           0xF9DE, /* H23 */
           0xF9AB, /* H24 */
           0x799A, /* H25 */
           0xF9AB, /* H26 */
           0xF9DE, /* H27 */
           0xFA2F, /* H28 */
           0xFA9B, /* H29 */
           0xFB1B, /* H30 */
           0xFBAA, /* H31 */
           0xFC3F, /* H32 */
           0xFCD5, /* H33 */
           0xFD66, /* H34 */
           0xFDEC, /* H35 */
           0xFE65, /* H36 */
           0xFECC, /* H37 */
           0xFF23, /* H38 */
           0xFF68, /* H39 */
           0xFF9D, /* H40 */
           0xFFC4, /* H41 */
           0xFFDF, /* H42 */
           0xFFF1, /* H43 */
           0xFFFB, /* H44 */
           0x0000, /* H45 */
           0x0002, /* H46 */
           0x0002, /* H47 */
           0x0001, /* H48 */
           0x0000, /* H49 */
           0x0000  /* H50 */

                                };

/*****************************************************************************/
/* End of blackman_high_pass_filter_1200Hz.h                                 */
/*****************************************************************************/  





