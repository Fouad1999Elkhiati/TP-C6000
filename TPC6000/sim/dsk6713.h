/*****************************************************************************/
/* dsk6713.h - SIMULATION version (no board).                                */
/* Replaces the Spectrum Digital DSK6713 board support library so that the   */
/* lab programs can be built and run on the CCS C6713 simulator.             */
/*****************************************************************************/
#ifndef DSK6713_SIM_H
#define DSK6713_SIM_H

typedef short          Int16;
typedef int            Int32;
typedef unsigned short Uint16;
typedef unsigned int   Uint32;

void DSK6713_init(void);

#endif
