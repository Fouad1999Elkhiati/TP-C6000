/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	IIR_filters_fourth_order.h                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for fourth order IIR filters type I and type II.            */
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

#ifndef IIR_FILTERS_FOURTH_ORDER_H
#define IIR_FILTERS_FOURTH_ORDER_H

short fourth_order_IIR_direct_form_I( const short * coefficients, 
                                                 short input);

short fourth_order_IIR_direct_form_II( const short * coefficients, 
                                                  short input);


#endif

/*****************************************************************************/
/* End of IIR_filters_fourth_order.h                                         */
/*****************************************************************************/
