/*****************************************************************************/
/* FILENAME                                                                  */
/*   FIR_filters.c                                                           */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Finite Impulse Response Filters using the TMS320C6713 DSK.              */
/*                                                                           */
/*  Rev 1.00   Richard Sikora                                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  HISTORY                                                                  */
/*  Rev 1.00                                                                 */
/*  23rd October 2026. Created by Richard Sikora from TMS320C5402 code.      */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>

#define FIR_FILTER_SIZE 51

/*****************************************************************************/
/* FIR_filter()                                                              */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* INPUTS: 1st parameter. Address of array of filter constants.              */
/*         2nd parameter. Latest input to filter                             */
/*                                                                           */
/* RETURNS: Output of FIR filter                                             */
/*                                                                           */ 
/*****************************************************************************/ 
                                                                             
                                                                             
short FIR_filter(const short * filter_constants, short input)
{
 static short value[FIR_FILTER_SIZE]; /* Retain value between calls */
 short return_value;
 short i;  /* Index into filter constants */ 
 int product;
 static short j = 0; /* Index into input array */
 
 product = 0;
 
 for ( i = 0 ; i < FIR_FILTER_SIZE ; i++)
 {
  /* Generate sum of products. Shift right to prevent overflow */
  /* This is first part of divide by 32767 */
  
  /* C6000: short x short is computed as a full 32-bit int product, i.e. */
  /* the 16x16->32 multiply the C5416 code was written for.               */
  product +=  ( (int)(value[j] * filter_constants[i]) >> 5);
  
  if ( j < FIR_FILTER_SIZE-1) /* Next item in circular buffer */
    { 
     j++;
    }                      
  else
    {
     j = 0;                    /* Go back to beginning of buffer */  
    }
 }
 
 if ( j < FIR_FILTER_SIZE -1) /* Loop done. Last filter element */
    {
     j++;
    }
 else
    {
     j = 0;   /* Point to new value */
    }

 product >>= 10;   /* Second part of divide by 32768 */
    
 value[j] = input; /* Read in new value for next time. */
 
 return_value = (short) product; 
 
 return(return_value); 
}        


/***************************************************************************/
/* Dual FIR Filter. Produces both bass and treble outputs                  */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* INPUTS:  1st parameter. Address of filter constants                     */
/*          2nd parameter. Latest input to filter                          */
/*                                                                         */
/* RETURNS: 32-bit int containing both bass and treble outputs             */
/*          (int is 32 bits on the C6000; long would be 40 bits)           */
/*          High word contains treble                                      */
/*          Low word contains bass                                         */
/*                                                                         */
/***************************************************************************/


int FIR_dual_filter( const short * filter_constants, short input) 
{
 static short value[FIR_FILTER_SIZE]; /* Retain value between calls */
 int output1;
 int output2;
 short i;  /* Index into filter constants */
 int product;
 static short j = 0; /* Index into input array */
 int mid_point;
 
 product = 0;

 for ( i = 0 ; i < FIR_FILTER_SIZE ; i++)
 {
   /* Multiply input by corresponding coefficient. Divide by 32 */
   /* to prevent overflow.                                      */
 
   product += (  (int)(value[j] * filter_constants[i]) >> 5);

   if ( i == ((FIR_FILTER_SIZE-1)/2) )
    {
     mid_point = value[j]; /* Not value[i] */
    }
  
   if ( j < FIR_FILTER_SIZE-1) /* Next item in circular buffer */
    {
     j++;
    }                    
   else
    {
     j = 0;    /* Go back to beginning of buffer */  
    }
 }
 
 /* One more increment of pointer is required */
 
 if ( j < FIR_FILTER_SIZE -1)
    {
     j++;
    }
 else
    {
     j = 0;   /* Point to new value */
    }
 
 product >>= 10; /* Remove remains of fractional part */   
 
 value[j] = input; /* Read in new value */
 
 output1 = (mid_point- product); /* First output to high word */
 output1 <<= 16; 
 
 output2 =  (product & 0xFFFF); /* Second output to low word */

 /* Combine two outputs into a single return value */
 
 return( output1 | output2 ); 

}      


/***************************************************************************/
/* End of FIR_filters.c                                                    */
/***************************************************************************/
