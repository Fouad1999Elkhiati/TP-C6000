/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 IIR_high_pass_filters.h                                                 */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for IIR low pass filters.                                   */
/*   Filters have been designed using the BLT from Butterworth filter.       */
/*                                                                           */
/*   The order of the coefficients is B0, B1/2, B2, A0, A1/2, A2, where      */
/*   B0, B1/2 and B2 are the numerator coefficients, A0, A1/2 and A2 are     */
/*   the denominator coefficients.                                           */
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

#ifndef IIR_HIGH_PASS_FILTERS_H
#define IIR_HIGH_PASS_FILTERS_H

/*****************************************************************************/
/* First order IIR high pass filters.                                        */
/*****************************************************************************/

const short first_order_high_pass_1000Hz[6] = { 30766, -30766, 32767, -28736 };

const short first_order_high_pass_2000Hz[6] = { 28955, -28955, 32767, -25143 };

const short first_order_high_pass_4000Hz[6] = { 25843, -25843, 32767, -18918 };


/*****************************************************************************/
/* Second order IIR high pass filters.                                       */
/*****************************************************************************/

/* Second order high pass filter 300 Hz */
const short IIR_high_pass_300Hz[6]  = { 31870, -31870, 31870, 
                                             32767, -31857, 30997   };

/* Second order high pass filter 600 Hz */
const short IIR_high_pass_600Hz[6]  = { 30997, -30997, 30997, 
                                             32767, -30949, 29322  };

/* Second order high pass filter 1000 Hz */
const short IIR_high_pass_1000Hz[6] = { 29870, -29870, 29870, 
                                             32767, -29742, 27330  };

/* Second order high pass filter 1200 Hz */
const short IIR_high_pass_1200Hz[6] = { 29322, -29322, 29322, 
                                             32767, -29135, 26240  };
                                             
/* Second order high pass filter 2000 Hz */
const short IIR_high_pass_2000Hz[6] = { 27226, -27226, 27226, 
                                             32767, -26754, 22629  };

/* Second order high pass filter 2400 Hz */
const short IIR_high_pass_2400Hz[6] = {  26233, -26233, 26233,
                                             32767,  -25575, 21015  };

/* Second order high pass filter 4000 Hz */
const short IIR_high_pass_4000Hz[6] = { 22587, -22587, 22587, 
                                             32767, -20964, 15649  };

/* Second order high pass filter 4800 Hz */
const short IIR_high_pass_4800Hz[6] = {  20936, -20936, 20936, 
                                             32767,  -18726, 13526  };

/* Second order high pass filter 9600 Hz */
const short IIR_high_pass_9600Hz[6] = {  12891, -12891, 12891, 
                                             32767,   -6053,  6416  };
                                             
#endif

/*****************************************************************************/
/* End of IIR_high_pass_filters.h                                            */
/*****************************************************************************/
