/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   sampling_rate.c                                                         */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Selecting the sampling rate number of the audio signal on the           */
/*   TMS320C6713 DSK.                                                        */
/*                                                                           */
/*   Takes an input from a microphone or from a CD player and feeds straight */
/*   through to loudspeakers. The user switches select the sampling rate     */
/*   between 2 kHz and 48 kHz.                                               */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*                                                                           */
/*   Revision 1.01															 */
/*   23rd October 2026. Sampling rate increased from 24 kHz to 48 kHz.       */
/*                                                                           */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora.                           */
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
    Uint16 counter = 0;   /* 16 bits as on the C54 (unsigned int), so it wraps at 65536 the same way */
              
    /* Start the codec */
    hCodec = DSK6713_AIC23_openCodec(0, &setup);
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_48KHZ);  /* Fs = 48 kHz as on the C5416 DSK */
 
    /* Display project details on StdOut. \n is important. */
 
    puts("TMS320C6713 DSK: Changing the sampling rate between 2 kHz and 48 kHz.\n");
          
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
   
      counter++; /* Increment counter every time through */
  
      if ( 0 == switch_value)
       {
         /* 48000 Hz sampling rate */
         left_output = left_input;  
         right_output = right_input;
       }
      else if ( 1 == switch_value)
       {
         /* 24000 Hz sampling rate. Use 48000 Hz divided by 2 */
         
         if ( 0 == ( counter % 2) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }   
       }  
      else if ( 2 == switch_value)
       {
         /* 16000 Hz sampling rate. Use 48000 Hz divided by 3 */
         
         if ( 0 == ( counter % 3) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }           
       } 
      else if ( 3 == switch_value)
       {
         /* 12000 Hz sampling rate. Use 48000 Hz divided by 4 */
         
         if ( 0 == ( counter % 4) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }         
       }       
      else if ( 4 == switch_value)
       {
         /* 9600 Hz sampling rate. Use 48000 Hz divided by 5 */
         
         if ( 0 == ( counter % 5) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }        
       }         
      else if ( 5 == switch_value)
       {
         /* 8000 Hz sampling rate. Use 48000 Hz divided by 6 */
         
         if ( 0 == ( counter % 6) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }         
       }   
      else if ( 6 == switch_value)
       {
         /* 6800 Hz sampling rate. Use 48000 Hz divided by 7 */
         
         if ( 0 == ( counter % 7) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }  
       }       
      else if ( 7 == switch_value)
       {
         /* 6000 Hz sampling rate. Use 48000 Hz divided by 8 */
         
         if ( 0 == ( counter % 8) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }         
       }        
      else if ( 8 == switch_value)
       {
         /* 5300 Hz sampling rate. Use 48000 Hz divided by 9 */
         
         if ( 0 == ( counter % 9) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }          
       }        
      else if ( 9 == switch_value )
       {
         /* 4800 Hz sampling rate. Use 48000 Hz divided by 10 */
         
         if ( 0 == ( counter % 10) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }          
       } 
      else if ( 10 == switch_value)
       {
         /* 4000 Hz sampling rate. Use 48000 Hz divided by 12 */
         
         if ( 0 == ( counter % 12) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }            
       } 
      else if ( 11 == switch_value)
       {
         /* 3000 Hz sampling rate. Use 48000 Hz divided by 16 */
         
         if ( 0 == ( counter % 16) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           }         
       }
      else if ( 12 == switch_value)
       {
         /* 2000 Hz sampling rate. Use 48000 Hz divided by 24 */
         
         if ( 0 == ( counter % 24) )
           {
             left_output = left_input;  
             right_output = right_input;
           }         
         else
           {
             /* Not time to update yet */
           } 
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
    
    puts("TMS320C6713 DSK program has terminated.\n");
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
/* End of sampling_rate.c                                                     */
/******************************************************************************/

