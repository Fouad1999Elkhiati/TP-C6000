/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	IIR_filters_second_order.c                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Infinite Impulse Response (IIR) filters second order type I and type II */
/*   Takes 3 numerator coefficients and 3 denominator coefficients.          */
/*                                                                           */
/*   Order of coefficients is numerator coefficients B0, B1/2, B2 then       */
/*   denominator coefficients A0, A1/2, A2.                                  */
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
/* A second order infinite impulse response (IIR) filter can be represented  */
/* by the following equation:                                                */
/*                                                                           */
/* H(z) = b0 + b1.z^-1 + b2.z^-2                                             */
/*        ----------------------                                             */
/*        a0 + a1.z^-1 + a2.z^-2                                             */  
/*                                                                           */ 
/* where H(z) is the transfer function. a0 is always 1.000                   */
/*                                                                           */
/* In order to implement this function on a fixed point processor, the value */
/* 32767 is used to represent 1.000                                          */
/*                                                                           */
/* Because the coefficients b1 and a1 can lie in the range -2.000 to +2.000, */
/* the coeffients supplied to the function are b1/2 and a1/2.                */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>

/* Numerator coefficients */
#define B0 0
#define B1 1
#define B2 2

/* Denominator coefficients */
#define A0 3
#define A1 4
#define A2 5


/*****************************************************************************/
/* second_order_IIR_direct_form_I()                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Second order direct form I IIR filter.                                    */
/*                                                                           */
/* This implmentation uses two buffers, one for x[n] and the other for y[n]  */
/*                                                                           */
/*****************************************************************************/

short second_order_IIR_direct_form_I( const short * coefficients, short input)
{
  int temp;
  static short x[3] = { 0, 0, 0 };  /* x(n), x(n-1), x(n-2). Must be static */
  static short y[3] = { 0, 0, 0 };  /* y(n), y(n-1), y(n-2). Must be static */ 

  x[0] = input; /* Copy input to x(n) */

  temp =  ( (int)coefficients[B0] * x[0]) ;   /* B0 * x(n)   */
  
  temp += ( (int)coefficients[B1] * x[1]);    /* B1/2 * x(n-1) */

  temp += ( (int)coefficients[B1] * x[1]);    /* B1/2 * x(n-1) */

  temp += ( (int)coefficients[B2] * x[2]);    /* B2 * x(n-2) */
  
  temp -= ( (int)coefficients[A1] * y[1]);    /* A1/2 * y(n-1) */
  
  temp -= ( (int) coefficients[A1] * y[1]);   /* A1/2 * y(n-1) */

  temp -= ( (int) coefficients[A2] * y[2]);   /* A2 * y(n-2) */
 
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
  
  y[2] = y[1];                 /* y(n-2) = y(n-1) */
  y[1] = y[0];                 /* y(n-1) = y(n)   */
    
  x[2] = x[1];                 /* x(n-2) = x(n-1) */
  x[1] = x[0];                 /* x(n-1) = x(n)   */

 return ( y[0] );
}

/*****************************************************************************/
/* second_order_IIR_direct_form_II()                                         */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Second order IIR (canonic) filter.                                        */
/*                                                                           */
/* Uses 32767 to represent 1.000.                                            */
/* Note that input is divided by 128 to prevent overload.                    */
/*                                                                           */
/*****************************************************************************/

short second_order_IIR_direct_form_II ( const short * coefficients, short input)
{
  int temp;
  static short delay[3] = { 0, 0, 0}; /* Must be static */

 /* Process using denominator coefficients */

  temp = (((int)coefficients[A0] * input ) >> 7 ); /* Divide by 128 */
  
  temp -= ((int)coefficients[A1] * delay[1] );  /* A1/2 */

  temp -= ((int)coefficients[A1] * delay[1] );  /* A1/2 */  

  temp -= ((int)coefficients[A2] * delay[2] );
  
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

  temp += ((int)coefficients[B1] * delay[1] ) ;  /* Add B1/2 */ 

  temp += ((int)coefficients[B1] * delay[1] ) ;  /* Add B1/2 */

  temp += ((int)coefficients[B2] * delay[2] ) ;  

  delay[2] = delay[1];
  delay[1] = (short)( delay[0] );

  /* Scale output. Divide by temp by coefficients[A0] then multiply by 128 */

  temp >>= ( 15 - 7); 

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
/* End of IIR_filters_second_order.c                                         */
/*****************************************************************************/

