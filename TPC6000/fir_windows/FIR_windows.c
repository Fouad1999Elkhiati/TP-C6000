/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	FIR_windows.c                                                            */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Program to show the effect of different window functions on the finite  */
/*   impulse response (FIR) filter using the TMS320C6713 DSK.                */
/*   Uses Rectangular, Hamming, Hanning, Blackman and Kaise windows to       */
/*   implement 1200 Hz high pass and low pass filters using 48000 Hz         */
/*   sampling rate.                                                          */
/*                                                                           */
/*   The bargraph is calibrated in units of 3 dB.                            */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision 1.10                                                           */
/*   23rd October 2026. Modified for 48000 Hz sampling rate.                 */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora from TMS320C5402 code.     */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>    /* Required for functions printf() and puts()          */



#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_led.h"
#include "dsk6713_dip.h"

#include "bargraph.h"
#include "switches.h"
#include "stereo.h"

#include "FIR_filters_asm.h"
#include "FIR_filters.h"

#include "Blackman_High_Pass_Filter_1200Hz.h"
#include "Hamming_High_Pass_Filter_1200Hz.h"  
#include "Hanning_High_Pass_Filter_1200Hz.h"
#include "Kaiser_High_Pass_Filter_1200Hz.h"
#include "Rectangular_High_Pass_Filter_1200Hz.h"


/*****************************************************************************/
/* Configuration of the AIC23 codec of the C6713 DSK                         */
/* (replaces the PCM3002 set-up of the C5416 DSK).                           */
/* Line in and headphone out at 0 dB, like the PCM3002 set-up (0x1FF).       */
/* The sampling rate is forced to 48000 Hz in UserTask(): all the filter     */
/* coefficients of this project were designed for Fs = 48000 Hz.            */
/*****************************************************************************/

DSK6713_AIC23_Config setup = {
    0x0017,  /* 0 LEFTINVOL  Left line input channel volume  : 0 dB          */
    0x0017,  /* 1 RIGHTINVOL Right line input channel volume : 0 dB          */
    0x01F9,  /* 2 LEFTHPVOL  Left headphone volume  : 0 dB                   */
    0x01F9,  /* 3 RIGHTHPVOL Right headphone volume : 0 dB                   */
    0x0011,  /* 4 ANAPATH    Analog path: line in -> ADC, DAC selected       */
    0x0000,  /* 5 DIGPATH    Digital path: no de-emphasis, no HP filter mute */
    0x0000,  /* 6 POWERDOWN  Everything powered on                           */
    0x0043,  /* 7 DIGIF      DSP mode, 16-bit samples, AIC23 is master       */
    0x0081,  /* 8 SAMPLERATE USB mode, 48 kHz                                */
    0x0001   /* 9 DIGACT     Digital interface active                        */
};

/*****************************************************************************/
/*                                                                           */
/* For compatibility with the 16-bit samples, these variables must          */
/* be declared as Int16 or short int, rather than int.                       */
/*                                                                           */
/*****************************************************************************/

Int16 left_input;
Int16 left_output; 
Int16 right_input;
Int16 right_output; 

/* C6000: CCS File I/O (probe points) transfers 32-bit words. mono_input is  */
/* therefore a 32-bit word, so that a probe point writing Length = 1 at      */
/* &mono_input cannot overwrite a neighbouring variable. Its value is always */
/* a 16-bit sample: the filter functions receive it as a short.              */
Int32 mono_input;

/*****************************************************************************/
/* UserTask()                                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* The main user task.                                                       */
/*                                                                           */
/* Called by main() (on the C5416 DSK it was scheduled by DSP/BIOS).        */
/*                                                                           */
/*****************************************************************************/
 
void UserTask()
{
    DSK6713_AIC23_CodecHandle hCodec;
    Uint32 sample;                 /* AIC23 read/write use 32-bit words */
    Uint32 i;
    unsigned int switch_value;
    Int32 dual_output;          /* 32 bits, as "long" on the C54 */
       
    /* Start the codec */
    hCodec = DSK6713_AIC23_openCodec(0, &setup);
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_48KHZ);  /* Fs = 48 kHz as on the C5416 DSK */
 
    /* Display project details on StdOut. \n is important. */
 
    puts("TMS320C6713 DSK: FIR filters using different window functions. Mono. Bargraph at 6dB intervals.\n");
    
    /* Clear buffers used for FIR filters to zero */      
    FIR_filters_asm_initialize();      
          
    for ( i = 0 ; i < 12000000 ; i++ )
    {
    
      /* Read left input channel */    
      while (!DSK6713_AIC23_read(hCodec, &sample));
      left_input = (Int16) sample;    

      /* Output to left output channel */       
      while (!DSK6713_AIC23_write(hCodec, (Uint32) left_output));

      /* Read right input channel */
      while (!DSK6713_AIC23_read(hCodec, &sample));
      right_input = (Int16) sample;

      /* Output to right output channel */
      while (!DSK6713_AIC23_write(hCodec, (Uint32) right_output));

      mono_input = stereo_to_mono( left_input, right_input);

      /* Read user switches on DSK and display their meaning on Stdout. */
      switch_value = switch_status_display();

      /* Display greater of two outputs on bargraph in 6 dB intervals. */
      bargraph_6dB ( left_output, right_output );
      
	  //switch_value=5; /* Pour test avec probe point */
      if ( 0 == switch_value)
        {
          /* Straight through for unfiltered reference */
          left_output = left_input >> 1;
          right_output = right_input >> 1;
        }
      else if ( 1 == switch_value)
        {
          /* 1200 Hz high pass FIR filter using rectangular windows */
          left_output = FIR_filter_asm ( &rectangular_high_pass_filter_1200Hz[0], mono_input); 
          right_output = FIR_filter ( &rectangular_high_pass_filter_1200Hz[0], mono_input);
        }
      else if ( 2 == switch_value)
        {
          /* 1200 Hz high pass FIR filter using Hamming window */
          left_output = FIR_filter_asm ( &hamming_high_pass_filter_1200Hz[0], mono_input);        
          right_output = FIR_filter ( &hamming_high_pass_filter_1200Hz[0], mono_input);        
        } 
      else if ( 3 == switch_value)
        {
          /* 1200 Hz high pass FIR filter using Hanning window */
          left_output = FIR_filter_asm ( &hanning_high_pass_filter_1200Hz[0], mono_input);          
          right_output = FIR_filter ( &hanning_high_pass_filter_1200Hz[0], mono_input);          
        }  
      else if ( 4 == switch_value)
        {
          /* 1200 Hz high pass FIR filter using Blackman window */
          left_output = FIR_filter_asm ( &blackman_high_pass_filter_1200Hz[0], mono_input);
          right_output = FIR_filter ( &blackman_high_pass_filter_1200Hz[0], mono_input);
        }
      else if ( 5 == switch_value)
        {
          /* 1200 Hz high pass FIR filter using Kaiser window */
          left_output = FIR_filter_asm ( &kaiser_high_pass_filter_1200Hz[0], mono_input);
          right_output = FIR_filter( &kaiser_high_pass_filter_1200Hz[0], mono_input);
        }         
      else if ( 6 == switch_value)
        {
          /* 1200 Hz low pass FIR filter using rectangular windows */
          dual_output = FIR_dual_filter_asm ( &rectangular_high_pass_filter_1200Hz[0] , mono_input);
          left_output = (Int16)( dual_output & 0xFFFF);
          right_output = left_output;
        }
      else if ( 7 == switch_value)
        {
          /* 1200 Hz low pass FIR filter using Hamming window */
          dual_output = FIR_dual_filter_asm ( &hamming_high_pass_filter_1200Hz[0], mono_input); 
          left_output = (Int16)( dual_output & 0xFFFF); 
          right_output = left_output;                
        } 
      else if ( 8 == switch_value)
        {
          /* 1200 Hz low pass FIR filter using Hanning window */
          dual_output = FIR_dual_filter_asm ( &hanning_high_pass_filter_1200Hz[0], mono_input);  
          left_output = (Int16)( dual_output & 0xFFFF);
          right_output = left_output;                  
        }  
      else if ( 9 == switch_value)
        {
          /* 1200 Hz low pass FIR filter using Blackman window */
          dual_output = FIR_dual_filter_asm ( &blackman_high_pass_filter_1200Hz[0], mono_input); 
          left_output = (Int16)( dual_output & 0xFFFF);
          right_output = left_output;          
        } 
      else if ( 10 == switch_value)
        {
          /* 1200 Hz low pass FIR filter using Kaiser window */
          left_output = FIR_dual_filter_asm ( &kaiser_high_pass_filter_1200Hz[0], mono_input);
          right_output = left_output;                     
        }        
      else if ( 11 == switch_value)
        {
          /* 1200 Hz low pass / high pass FIR filter using rectangular windows */
          dual_output = FIR_dual_filter_asm ( &rectangular_high_pass_filter_1200Hz[0] , mono_input);
          left_output = (Int16)( dual_output >> 16);
          right_output = (Int16)( dual_output & 0xFFFF );
          left_output >>= 1; /* Attenuate treble */
        }
      else if ( 12 == switch_value)
        {
          left_output = left_input >> 1;
          right_output = right_input >> 1;
        } 
      else if ( 13 == switch_value)
        {
          left_output = left_input >> 1;
          right_output = right_input >> 1;
        }  
      else if ( 14 == switch_value)
        {
          left_output = left_input >> 1;
          right_output = right_input >> 1;
        } 
      else if ( 15 == switch_value)
        {
          left_output = left_input >> 1;
          right_output = right_input >> 1;
        }
     }

    /* Finished processing. Close the codec */
    DSK6713_AIC23_closeCodec(hCodec);
    
    puts("TMS320C6713 DSK has terminated.\n");
}

/*****************************************************************************/
/* main()                                                                    */
/*****************************************************************************/

void main()
{
    /* Initialize the board support library, the LEDs and the DIP switches */

    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();

    /* On the C5416 DSK UserTask() was scheduled by DSP/BIOS.               */
    /* This C6713 version does not use DSP/BIOS: UserTask() is called here. */

    UserTask();
}

/******************************************************************************/
/* End of FIR_windows.c                                                       */
/******************************************************************************/

