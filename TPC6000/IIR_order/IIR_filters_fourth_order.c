/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	IIR_filters_fourth_order.c                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Infinite Impulse Response (IIR) filters fourth order type I and type II */
/*   Takes 3 numerator coefficients and 3 denominator coefficients.          */
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
/* An second infinite impulse response (IIR) filter can be represented by    */
/* the following equation:                                                   */
/*                                                                           */
/* H(z) = b0 + b1.z^-1 + b2.z^-2                                             */
/*        ----------------------                                             */
/*        a0 + a1.z^-1 + a2.z^-2                                             */  
/*                                                                           */ 
/* where H(z) is the transfer function. a0 is always 1.000                   */
/*                                                                           */
/* To implement a fourth order filter, two of these stages are cascaded.     */
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
/* fourth_order_IIR_direct_form_I()                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Fourth order direct form I IIR filter implemented by cascading two second */
/* order filters.                                                            */
/*                                                                           */
/* This implementation uses two buffers, one for x[n] and the other for y[n] */
/*                                                                           */
/*****************************************************************************/

short fourth_order_IIR_direct_form_I( const short * coefficients, short input)
{
  int temp;
  static short x[2][3] = { 0, 0, 0, 0, 0, 0 };  /* x(n), x(n-1), x(n-2). Must be static */
  static short y[2][3] = { 0, 0, 0, 0, 0, 0 };  /* y(n), y(n-1), y(n-2). Must be static */ 
  unsigned int stages;

  temp = (int) input; /* Copy input to temp */

  for ( stages = 0 ; stages < 2 ; stages++)
    {
     x[stages][0] = (short) temp; /* Copy input to x[stages][0] */
    
     temp =  ( (int)coefficients[B0] * x[stages][0]) ;   /* B0 * x(n)   */
  
     temp += ( (int)coefficients[B1] * x[stages][1]);    /* B1/2 * x(n-1) */

     temp += ( (int)coefficients[B1] * x[stages][1]);    /* B1/2 * x(n-1) */

     temp += ( (int)coefficients[B2] * x[stages][2]);    /* B2 * x(n-2) */
  
     temp -= ( (int)coefficients[A1] * y[stages][1]);    /* A1/2 * y(n-1) */
  
     temp -= ( (int) coefficients[A1] * y[stages][1]);   /* A1/2 * y(n-1) */

     temp -= ( (int) coefficients[A2] * y[stages][2]);   /* A2 * y(n-2) */
 
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

     y[stages][0] = (short) ( temp );

     /* Shuffle values along one place for next time */
  
     y[stages][2] = y[stages][1];   /* y(n-2) = y(n-1) */
     y[stages][1] = y[stages][0];   /* y(n-1) = y(n)   */
    
     x[stages][2] = x[stages][1];   /* x(n-2) = x(n-1) */
     x[stages][1] = x[stages][0];   /* x(n-1) = x(n)   */

     /* temp is used as input next time through */
   }

 return ( (short) temp ); 
}


/*****************************************************************************/
/* fourth_order_IIR_direct_form_II()                                         */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Cascades two second order IIR filters.                                    */
/* Uses 32767 to represent 1.000.                                            */
/* Note that input is divided by 64 to prevent overload, especially with     */
/* bandpass filters.                                                         */
/*                                                                           */
/*****************************************************************************/

short fourth_order_IIR_direct_form_II ( const short * coefficients, short input)
{
  int temp;
  static short delay[2][3] = { 0, 0, 0, 0, 0, 0};
  unsigned int stages;

  /* Copy input to temp for temporary storage */

  temp = (int) input; 

  for ( stages = 0 ; stages < 2 ; stages++)
    {
      /* Process denominator coefficients */

     delay[stages][0] = (short) temp;

     temp = (((int)coefficients[A0] * delay[stages][0] ) >> 6); /* Divide by 64 */
  
     temp -= ((int)coefficients[A1] * delay[stages][1] );  /* A1/2 */

     temp -= ((int)coefficients[A1] * delay[stages][1] );  /* A1/2 */  

     temp -= ((int)coefficients[A2] * delay[stages][2] );
  
     temp >>= 15;  /* Divide temp by coefficients[A0] */

     if ( temp > 32767)
       {
         temp = 32767;
       }
     else if ( temp < -32767)
       {
         temp = -32767;
       }  

     delay[stages][0] = ( short ) temp;

     /* Process numerator coefficients */

     temp = ((int)coefficients[B0] * delay[stages][0] );

     temp += ((int)coefficients[B1] * delay[stages][1] ) ;  /* B1/2 */ 

     temp += ((int)coefficients[B1] * delay[stages][1] ) ;  /* B1/2 */

     temp += ((int)coefficients[B2] * delay[stages][2] ) ;  

     delay[stages][2] = delay[stages][1];
     delay[stages][1] = delay[stages][0];

     /* Divide temp by coefficients[A0] then multiply by 64 */

     temp >>= ( 15 - 6 );

     /* Range limit temp between maximum and minimum */

     if ( temp > 32767)
       {
         temp = 32767;
       }
     else if ( temp < -32767)
       {
         temp = -32767;
       }  

     /* Temp will be fed into input of filter next time through */
    }

  return ( (short) temp ); 
}


/*****************************************************************************/
/* End of IIR_filters_fourth_order.c                                         */
/*****************************************************************************/

