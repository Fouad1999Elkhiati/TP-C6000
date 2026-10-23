/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   sampling_bits.c                                                         */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Selecting the number of bits resolution of the ADC / DAC on the         */
/*   TMS320C6713 DSK.                                                        */
/*                                                                           */
/*   Takes an input from a microphone or from a CD player and feeds straight */
/*   through to loudspeakers. The user switches select the number of bits of */
/*   resolution of the ADC/DAC.                                              */
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

#include <stdio.h>    /* Required for functions printf() and puts()          */


#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_led.h"
#include "dsk6713_dip.h"

#include "bargraph.h"
#include "switches.h"


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
    Int32 i;
    unsigned int switch_value; 
        
    /* Start the codec */
    hCodec = DSK6713_AIC23_openCodec(0, &setup);
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_48KHZ);  /* Fs = 48 kHz as on the C5416 DSK */
 
    /* Display project details on StdOut. \n is important. */
 
    puts("TMS320C6713 DSK: Changing the number of bits per sample.\n");
          
    for (i = 0; i < 12000000; i++)
    {
      /* Read left input channel */    
      while (!DSK6713_AIC23_read(hCodec, &sample));
      left_input = (Int16) sample;

      /* Write to left output channel */      
      while (!DSK6713_AIC23_write(hCodec, (Uint32) left_output));
      
      /* Read right input channel */
      while (!DSK6713_AIC23_read(hCodec, &sample));
      right_input = (Int16) sample;
      
      /* Write to right output channel */
      while (!DSK6713_AIC23_write(hCodec, (Uint32) right_output));
      
      /* Read user switches on DSK and display their meaning on Stdout. */
      switch_value = switch_status_display();
  
      if ( 0 == switch_value)
       {
         /* 16-bit resolution */
         left_output = left_input;  
         right_output = right_input;
       }
      else if ( 1 == switch_value)
       {
         /* 15-bit resolution */
         left_output = ( left_input & 0xFFFE );  
         right_output = ( right_input & 0xFFFE );         
       }  
      else if ( 2 == switch_value)
       {
         /* 14-bit resolution */
         left_output = ( left_input & 0xFFFC );  
         right_output = ( right_input & 0xFFFC );           
       } 
      else if ( 3 == switch_value)
       {
         /* 13-bit resolution */
         left_output = ( left_input & 0xFFF8 );  
         right_output = ( right_input & 0xFFF8 );          
       }       
      else if ( 4 == switch_value)
       {
         /* 12-bit resolution */
         left_output = ( left_input & 0xFFF0 );  
         right_output = ( right_input & 0xFFF0 );        
       }         
      else if ( 5 == switch_value)
       {
         /* 11-bit resolution */
         left_output = ( left_input & 0xFFE0 );  
         right_output = ( right_input & 0xFFE0 );          
       }   
      else if ( 6 == switch_value)
       {
         /* 10-bit resolution */
         left_output = ( left_input & 0xFFC0 );  
         right_output = ( right_input & 0xFFC0 );  
       }       
      else if ( 7 == switch_value)
       {
         /* 9-bit resolution */
         left_output = ( left_input & 0xFF80 );  
         right_output = ( right_input & 0xFF80 );          
       }        
      else if ( 8 == switch_value)
       {
         /* 8-bit resolution */
         left_output = ( left_input & 0xFF00 );  
         right_output = ( right_input & 0xFF00 );           
       }        
      else if ( 9 == switch_value )
       {
         /* 7-bit resolution */
         left_output = ( left_input & 0xFE00 );  
         right_output = ( right_input & 0xFE00 );           
       } 
      else if ( 10 == switch_value)
       {
         /* 6-bit resolution */
         left_output = ( left_input & 0xFC00 );  
         right_output = ( right_input & 0xFC00 );           
       } 
      else if ( 11 == switch_value)
       {
         /* 5-bit resolution */
         left_output = ( left_input & 0xF800 );  
         right_output = ( right_input & 0xF800 );         
       }
      else if ( 12 == switch_value)
       {
          /* Not used */
       }        
      else if ( 13 == switch_value)
       {
          /* Not used */
       }
      else if ( 14 == switch_value)
       {
          /* Not used */
       }        
      else if ( 15 == switch_value)
       {
          /* Not used */
       }               
  
      /* Display greater of two outputs on bargraph */
      bargraph_6dB ( left_output, right_output);
     }

    /* Finished processing. Close the codec */
    DSK6713_AIC23_closeCodec(hCodec);
    
    puts("TMS320C6713 DSK program has terminated\n");
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
/* End of sampling_bits.c                                                     */
/******************************************************************************/

