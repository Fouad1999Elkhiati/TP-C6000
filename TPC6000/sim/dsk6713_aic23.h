/* dsk6713_aic23.h - SIMULATION version: the codec reads a test signal and   */
/* records the outputs in arrays (see sim_board.c).                          */
#ifndef DSK6713_AIC23_SIM_H
#define DSK6713_AIC23_SIM_H

typedef struct { int regs[10]; } DSK6713_AIC23_Config;
typedef int DSK6713_AIC23_CodecHandle;

#define DSK6713_AIC23_FREQ_8KHZ   1
#define DSK6713_AIC23_FREQ_16KHZ  2
#define DSK6713_AIC23_FREQ_24KHZ  3
#define DSK6713_AIC23_FREQ_32KHZ  4
#define DSK6713_AIC23_FREQ_44KHZ  5
#define DSK6713_AIC23_FREQ_48KHZ  6
#define DSK6713_AIC23_FREQ_96KHZ  7

DSK6713_AIC23_CodecHandle DSK6713_AIC23_openCodec(int id, DSK6713_AIC23_Config *Config);
void  DSK6713_AIC23_closeCodec(DSK6713_AIC23_CodecHandle hCodec);
void  DSK6713_AIC23_setFreq(DSK6713_AIC23_CodecHandle hCodec, Uint32 freq);
Int16 DSK6713_AIC23_read(DSK6713_AIC23_CodecHandle hCodec, Uint32 *val);
Int16 DSK6713_AIC23_write(DSK6713_AIC23_CodecHandle hCodec, Uint32 val);

#endif
