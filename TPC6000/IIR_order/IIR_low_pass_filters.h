/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 IIR_low_pass_filters.h                                                  */
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

#ifndef IIR_LOW_PASS_FILTERS_H
#define IIR_LOW_PASS_FILTERS_H

/*****************************************************************************/
/* First order IIR low pass filters.                                         */
/*****************************************************************************/

const short first_order_low_pass_1000Hz[6] = { 2015, 2015, 32767, -28736 };

const short first_order_low_pass_2000Hz[6] = {3812, 3812, 32767, -25143 };

const short first_order_low_pass_4000Hz[6] = { 6924, 6924, 32767, -18918 };

/*****************************************************************************/
/* Second order IIR low pass filters.                                        */
/*****************************************************************************/

/* Second order low pass filter 300 Hz */
const short IIR_low_pass_300Hz[6]  = {    13,     13,    13, 
                                            32767, -31857,  30997 };

/* Second order low pass filter 600 Hz */
const short IIR_low_pass_600Hz[6]  = {    48,     48,    48, 
                                            32767, -30949, 29322  };

/* Second order low pass filter 1000 Hz */
const short IIR_low_pass_1000Hz[6] = {   128,    128,   128, 
                                            32767, -29742, 27330  };

/* Second order low pass filter 1200 Hz */
const short IIR_low_pass_1200Hz[6] = {   181,    181,   181, 
                                            32767, -29096, 26150  };

/* Second order low pass filter 2000 Hz */
const short IIR_low_pass_2000Hz[6] = {    472,    472,   472,
                                             32767, -26754, 22629  };

/* Second order low pass filter 2400 Hz */
const short IIR_low_pass_2400Hz[6] = {    658,    658,   658,
                                             32767, -25575, 21015  };

/* Second order low pass filter 4000 Hz */
const short IIR_low_pass_4000Hz[6] = {   1622,   1622,  1622, 
                                             32767, -20964, 15649  };

/* Second order low pass filter 4800 Hz */
const short IIR_low_pass_4800Hz[6] = {   2210,   2210,  2210, 
                                             32767, -18726, 13526  };

/* Second order low pass filter 9600 Hz */
const short IIR_low_pass_9600Hz[6] = {   6769,   6769,  6768, 
                                             32767,  -6053,  6416  };
                                             
#endif

/*****************************************************************************/
/* End of IIR_low_pass_filters.h                                             */
/*****************************************************************************/
