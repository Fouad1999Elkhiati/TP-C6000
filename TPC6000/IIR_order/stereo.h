/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */ 
/* 	 stereo.h                                                                */                           
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Header file for converting from stereo to mono.                         */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */ 
/*   Author  : Fouad El Khiati                                               */ 
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora from TMS320C5402 code.     */
/*                                                                           */
/*****************************************************************************/

#ifndef STEREO_H
#define STEREO_H

short stereo_to_mono(short left_channel, short right_channel);

#endif

/*****************************************************************************/
/* End of stereo.h                                                           */
/*****************************************************************************/
