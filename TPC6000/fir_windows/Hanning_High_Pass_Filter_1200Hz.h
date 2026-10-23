/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	hanning_high_pass_filter_1200Hz.h                                        */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file FIR highpass filter with 1200 Hz cutoff using a             */
/*   sampling frequency of 48000 Hz and a hanning window.                    */
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

const short hanning_high_pass_filter_1200Hz[] = {

         0x0000, /* H0 */
         0x0001, /* H1 */
         0x0003, /* H2 */
         0x0005, /* H3 */
         0x0005, /* H4 */
         0x0000, /* H5 */
         0xFFF4, /* H6 */
         0xFFE0, /* H7 */
         0xFFBF, /* H8 */
         0xFF92, /* H9 */
         0xFF56, /* H10 */
         0xFF0B, /* H11 */
         0xFEB1, /* H12 */
         0xFE49, /* H13 */
         0xFDD4, /* H14 */
         0xFD55, /* H15 */
         0xFCD0, /* H16 */
         0xFC48, /* H17 */
         0xFBC1, /* H18 */
         0xFB40, /* H19 */
         0xFACA, /* H20 */
         0xFA62, /* H21 */
         0xFA0D, /* H22 */
         0xF9CE, /* H23 */
         0xF9A7, /* H24 */
         0x799A, /* H25 */
         0xF9A7, /* H26 */
         0xF9CE, /* H27 */
         0xFA0D, /* H28 */
         0xFA62, /* H29 */
         0xFACA, /* H30 */
         0xFB40, /* H31 */
         0xFBC1, /* H32 */
         0xFC48, /* H33 */
         0xFCD0, /* H34 */
         0xFD55, /* H35 */
         0xFDD4, /* H36 */
         0xFE49, /* H37 */
         0xFEB1, /* H38 */
         0xFF0B, /* H39 */
         0xFF56, /* H40 */
         0xFF92, /* H41 */
         0xFFBF, /* H42 */
         0xFFE0, /* H43 */
         0xFFF4, /* H44 */
         0x0000, /* H45 */
         0x0005, /* H46 */
         0x0005, /* H47 */
         0x0003, /* H48 */
         0x0001, /* H49 */
         0x0000  /* H50 */
                                };

/*****************************************************************************/
/* End of hanning_high_pass_filter_1200Hz.h                                  */
/*****************************************************************************/  





