/*****************************************************************************/
/* sim_board.c - Simulated C6713 DSK for running the lab programs WITHOUT A  */
/*               BOARD, on the Code Composer Studio C6713 simulator.         */
/*                                                                           */
/* - The codec "reads" a test signal (same sample on left and right).        */
/* - What the program writes to the codec is stored in sim_left[] and        */
/*   sim_right[]; the input is stored in sim_in[]. Plot them with            */
/*   View > Graph > Time/Frequency (16-bit signed integer, size SIM_N).      */
/* - The DIP switches are the variable sim_switches (0..15).                 */
/* - The LEDs are the 4 low bits of the variable sim_leds.                   */
/* - After SIM_N samples the results are written to sim_output.csv, a        */
/*   summary is printed in the Stdout window and the program stops (exit).   */
/*                                                                           */
/* The program files of the lab are NOT modified: only the board library     */
/* (dsk6713bsl.lib) is replaced by this file.                                */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"

/*===========================================================================*/
/* SETTINGS: change them here (then Rebuild), or in a Watch window after     */
/* File > Load Program and before Run.                                       */
/*===========================================================================*/

volatile Uint32 sim_switches  = 1;      /* DIP switch value 0..15 = filter  */

volatile Uint32 sim_signal    = 0;      /* 0 = impulse (one sample)          */
                                        /* 1 = step                          */
                                        /* 2 = sine at sim_freq_hz           */
                                        /* 3 = white noise                   */
                                        /* 4 = sweep (chirp) 0 Hz -> 24 kHz  */

volatile Int32  sim_amplitude = 0x4000; /* amplitude (0x4000 as in the lab   */
                                        /* files infir.dat / inpulse.dat)    */
volatile Uint32 sim_freq_hz   = 1000;   /* frequency of the sine (Hz)        */

#define SIM_N       512                 /* number of samples recorded        */
#define SIM_FS      48000.0             /* sampling frequency (Hz)           */

/* switches.c only accepts a new switch value after two identical readings  */
/* 4801 samples apart. The test signal therefore starts after SIM_WARMUP    */
/* samples of silence, once the selected filter is active.                  */
#define SIM_WARMUP  5000

/*===========================================================================*/
/* RESULTS                                                                   */
/*===========================================================================*/

Int16  sim_in[SIM_N];                   /* test signal                       */
Int16  sim_left[SIM_N];                 /* left output  y_left(n)            */
Int16  sim_right[SIM_N];                /* right output y_right(n)           */
volatile Uint32 sim_leds = 0;           /* LED 0..3 = bit 0..3               */

static Uint32 n_read  = 0;              /* number of codec reads  (L and R)  */
static Uint32 n_write = 0;              /* number of codec writes (L and R)  */
static Uint32 noise   = 12345;

static Int16 test_signal(Uint32 k)
{
    double t = (double) k / SIM_FS;
    double v;

    switch (sim_signal)
    {
    case 0:  v = (k == 0) ? sim_amplitude : 0;                        break;
    case 1:  v = sim_amplitude;                                       break;
    case 2:  v = sim_amplitude * sin(2.0 * 3.14159265358979 * sim_freq_hz * t);
             break;
    case 3:  noise = noise * 1103515245u + 12345u;
             v = ((double)((Int32)(noise >> 16) - 32768) / 32768.0) * sim_amplitude;
             break;
    default: /* linear chirp from 0 to SIM_FS/2 over SIM_N samples */
             v = sim_amplitude * sin(3.14159265358979 * (SIM_FS / 2.0) * t * t
                                     / ((double) SIM_N / SIM_FS));
             break;
    }
    if (v >  32767.0) v =  32767.0;
    if (v < -32768.0) v = -32768.0;
    return (Int16) v;
}

static void sim_finish(void)
{
    FILE *f;
    int k;
    Int32 peak_l = 0, peak_r = 0;

    for (k = 0; k < SIM_N; k++)
    {
        if (abs(sim_left[k])  > peak_l) peak_l = abs(sim_left[k]);
        if (abs(sim_right[k]) > peak_r) peak_r = abs(sim_right[k]);
    }
    printf("SIMULATION DONE: switches = %u, signal = %u, %d samples.\n",
           sim_switches, sim_signal, SIM_N);
    printf("Peak |left| = %ld, peak |right| = %ld, LEDs = 0x%X.\n",
           (long) peak_l, (long) peak_r, sim_leds);
    printf("Graph sim_in, sim_left, sim_right (16-bit signed, size %d).\n", SIM_N);

    f = fopen("sim_output.csv", "w");
    if (f != NULL)
    {
        fprintf(f, "n,input,left,right\n");
        for (k = 0; k < SIM_N; k++)
            fprintf(f, "%d,%d,%d,%d\n", k, sim_in[k], sim_left[k], sim_right[k]);
        fclose(f);
        printf("Results written to sim_output.csv\n");
    }
    exit(0);                            /* the simulator halts here          */
}

/*===========================================================================*/
/* Board functions used by the lab programs                                 */
/*===========================================================================*/

void DSK6713_init(void)       { }
void DSK6713_DIP_init(void)   { }
void DSK6713_LED_init(void)   { sim_leds = 0; }

Uint32 DSK6713_DIP_get(Uint32 dipNum)
{
    return (sim_switches >> dipNum) & 1;      /* 1 = switch on */
}

void DSK6713_LED_on(Uint32 ledNum)     { sim_leds |=  (1u << ledNum); }
void DSK6713_LED_off(Uint32 ledNum)    { sim_leds &= ~(1u << ledNum); }
void DSK6713_LED_toggle(Uint32 ledNum) { sim_leds ^=  (1u << ledNum); }

DSK6713_AIC23_CodecHandle DSK6713_AIC23_openCodec(int id, DSK6713_AIC23_Config *Config)
{
    n_read = n_write = 0;
    return 1;
}

void DSK6713_AIC23_closeCodec(DSK6713_AIC23_CodecHandle hCodec) { }
void DSK6713_AIC23_setFreq(DSK6713_AIC23_CodecHandle hCodec, Uint32 freq) { }

/* Reads alternate left / right, as on the board */
Int16 DSK6713_AIC23_read(DSK6713_AIC23_CodecHandle hCodec, Uint32 *val)
{
    Uint32 iteration = n_read / 2;
    Int16  x = 0;

    if (iteration >= SIM_WARMUP)
    {
        Uint32 k = iteration - SIM_WARMUP;
        if (k < SIM_N)
        {
            x = test_signal(k);
            if ((n_read & 1) == 0) sim_in[k] = x;
        }
    }
    n_read++;
    *val = (Uint16) x;                  /* 16-bit sample, as the McBSP gives */
    return 1;
}

/* Writes alternate left / right. The value written during iteration i was  */
/* computed from the input of iteration i-1 (the programs write first and    */
/* filter afterwards), so it is stored at index i-1.                         */
Int16 DSK6713_AIC23_write(DSK6713_AIC23_CodecHandle hCodec, Uint32 val)
{
    Uint32 iteration = n_write / 2;
    Int32  k = (Int32) iteration - 1 - SIM_WARMUP;

    if (k >= 0 && k < SIM_N)
    {
        if ((n_write & 1) == 0) sim_left[k]  = (Int16) val;
        else                    sim_right[k] = (Int16) val;
    }
    n_write++;
    if (k == SIM_N - 1 && (n_write & 1) == 0)
        sim_finish();                   /* last right sample written */
    return 1;
}
