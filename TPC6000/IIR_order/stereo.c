/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	stereo.c                                                                 */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Conversion from stereo to mono.                                         */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision: 1.00                                                          */
/*   23rd October 2026. Created by Richard Sikora.                           */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>


/*****************************************************************************/
/* stereo_to_mono()                                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* INPUTS:  left and right channel (stereo) information.                     */
/*                                                                           */
/* RETURNS: Average of two input channels (mono)                             */
/*                                                                           */
/*****************************************************************************/

short stereo_to_mono(short left_channel, short right_channel)
{
 int temp;
 
 /* Take average of left and right channels. */
 
 temp = (int) left_channel + (int) right_channel;
 
 temp >>= 1;    /* Divide by 2 to prevent overload at output */
 
 return ((short) temp); /* Return mono value to calling function */
}

/*****************************************************************************/
/* End of stereo.c                                                           */
/*****************************************************************************/
