/*****************************************************************************/
/* FILENAME                                                                  */
/*   FIR_filters.h                                                           */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header File for different Finite Impulse Response filters               */
/*                                                                           */
/*****************************************************************************/

#ifndef FIR_FILTERS_H
#define FIR_FILTERS_H

short FIR_filter(const short * filter_constants, short input);
int FIR_dual_filter( const short * filter_constants, short input);

#endif

/******************************************************************************/
/* End of FIR_filters.h                                                       */
/******************************************************************************/

  
