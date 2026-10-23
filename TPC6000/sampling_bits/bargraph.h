/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	bargraph.h                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Header file for using four user LEDs as a bargraph display.             */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora from TMS320C5402 DSK code. */
/*                                                                           */
/*****************************************************************************/

#ifndef BARGRAPH_H
#define BARGRAPH_H

 void bargraph_3dB(signed int left_channel, signed int right_channel);

 void bargraph_6dB(signed int left_channel, signed int right_channel);
 
 void bargraph_12dB(signed int left_channel, signed int right_channel); 

 void spectrum_analyser_display( unsigned int input1,
                                 unsigned int input2, 
                                 unsigned int input3,
                                 unsigned int input4 );

#endif

/*****************************************************************************/
/* End of bargraph.h                                                         */
/*****************************************************************************/
