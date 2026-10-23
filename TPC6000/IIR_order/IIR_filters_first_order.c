/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	IIR_filters_first_order.c                                                */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   First order Infinite Impulse Response (IIR) filters type I and type II. */
/*   Takes 2 numerator coefficients and 2 denominator coefficients.          */
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

/*****************************************************************************/
/*                                                                           */
/* A first order infinite impulse response (IIR) filter can be represented   */
/* by the following equation:                                                */
/*                                                                           */
/* H(z) = b0 + b1.z^-1                                                       */
/*        ------------                                                       */
/*        a0 + a1.z^-1                                                       */  
/*                                                                           */ 
/* where H(z) is the transfer function. a0 is always 1.000                   */
/*                                                                           */
/* In order to implement this function on a fixed point processor, the value */
/* 32767 is used to represent 1.000                                          */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>

/* Numerator coefficients */
#define B0 0
#define B1 1


/* Denominator coefficients */
#define A0 2
#define A1 3


/*****************************************************************************/
/* first_order_IIR_direct_form_I()                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* First order direct form I IIR filter.                                     */
/*                                                                           */
/* PARAMETER 1: Coefficients in order B0 B1 A0 A1.                           */
/*                                                                           */
/* This implmentation uses two buffers, one for x[n] and the other for y[n]  */
/*                                                                           */
/*****************************************************************************/


short first_order_IIR_direct_form_I( const short * coefficients, short input)
{
  int temp;
  static short x[2] = { 0, 0 };  /* x(n), x(n-1). Must be static */
  static short y[2] = { 0, 0 };  /* y(n), y(n-1). Must be static */ 

  x[0] = input; /* Copy input to x(n) */

  temp =  ( (int) coefficients[B0] * x[0]) ;   /* B0 * x(n)   */
  
  temp += ( (int) coefficients[B1] * x[1]);    /* B1 * x(n-1) */

  temp -= ( (int) coefficients[A1] * y[1]);    /* A1 * y(n-1) */
 
  /* Divide temp by coefficients[A0] */    

  temp >>= 15;

  /* Range limit temp between maximum and minimum */

  if ( temp > 32767 )
   {
     temp = 32767;
   }
  else if ( temp < -32767)
   {
     temp = -32767;
   }

  y[0] = (short) ( temp );

  /* Shuffle values along one place for next time */
  
  y[1] = y[0];                 /* y(n-1) = y(n)   */
    
  x[1] = x[0];                 /* x(n-1) = x(n)   */

 return ( y[0] );
}


/*****************************************************************************/
/* first_order_IIR_direct_form_II()                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* First order IIR (canonic) filter.                                         */
/*                                                                           */
/* Uses 32767 to represent 1.000.                                            */
/* Note that input is divided by 2 to prevent overload.                      */
/*                                                                           */
/*****************************************************************************/

short first_order_IIR_direct_form_II ( const short * coefficients, short input)
{
  int temp;
  static short delay[2] = { 0, 0 }; /* Must be static */

 /* Process using denominator coefficients */

  temp = (((int)coefficients[A0] * input ) >> 1 ); /* Divide by 2 */
  
  temp -= ((int)coefficients[A1] * delay[1] );  /* A1 */

  temp >>= 15;  /* temp /= coefficients[A0] */

  /* Limit output to maximum and minimum */

  if ( temp > 32767)
   {
     temp = 32767;
   }
  else if ( temp < -32767)
   {
     temp = -32767;
   } 

  delay[0] = (short) temp;

  /* Process using numerator coefficients */

  temp = ((int)coefficients[B0] * delay[0] );

  temp += ((int)coefficients[B1] * delay[1] ) ;  /* B1 */ 

  delay[1] = (short)( delay[0] );

  /* Scale output. Divide by temp by coefficients[A0] then multiply by 2 */

  temp >>= ( 15 - 1 ); 

  /* Range limit output between maximum and minimum */

  if ( temp > 32767 )
   {
     temp = 32767;
   }
  else if ( temp < -32767)
   {
     temp = -32767;
   } 

  return ( (short) temp ); 
}                        


/*****************************************************************************/
/* End of IIR_filters_first_order.c                                          */
/*****************************************************************************/

