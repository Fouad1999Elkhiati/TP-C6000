/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	IIR_filters_first_order.h                                                */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for first order IIR filters type I and type II.             */
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

#ifndef IIR_FILTERS_FIRST_ORDER_H
#define IIR_FILTERS_FIRST_ORDER_H

short first_order_IIR_direct_form_I( const short * coefficients, 
                                                 short input);

short first_order_IIR_direct_form_II( const short * coefficients, 
                                                  short input);

#endif

/*****************************************************************************/
/* End of IIR_filters_first_order.h                                          */
/*****************************************************************************/
