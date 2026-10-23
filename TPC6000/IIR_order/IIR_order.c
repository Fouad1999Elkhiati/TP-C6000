/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 IIR_order.c                                                             */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Infinite Impulse Response filters (IIR) from 1st order to fifth order.  */
/*                                                                           */
/*   Takes an input from a microphone or from a CD player and feeds through  */
/*   through low pass and high pass IIR filters to loudspeakers.             */
/*   Bargraph calibrated in steps of 3 dB.                                   */
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

#include <stdio.h>    /* Required for functions printf() and puts()          */



#include "dsk6713.h"
#include "dsk6713_aic23.h"
#include "dsk6713_led.h"
#include "dsk6713_dip.h"

#include "bargraph.h"
#include "switches.h"
#include "stereo.h"

#include "IIR_filters_first_order.h"
#include "IIR_filters_second_order.h"
#include "IIR_filters_fourth_order.h"

#include "IIR_low_pass_filters.h"
#include "IIR_high_pass_filters.h"

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
Int16 mono_input; 


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
    unsigned int switch_value = 0;  /* was read uninitialised on the first sample */
	Int16 temp;
	
    /* Start the codec */
    hCodec = DSK6713_AIC23_openCodec(0, &setup);
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_48KHZ);  /* Fs = 48 kHz as on the C5416 DSK */
 
    /* Display project details on StdOut. \n is important. */
 
    puts("TMS320C6713 DSK: IIR Filters, first order to fifth order. Bargraph at 6 dB intervals.\n");
  
    for ( i = 0 ; i < 12000000 ; i++ )
    {
      /* Read left input channel */    
      while (!DSK6713_AIC23_read(hCodec, &sample));
      left_input = (Int16) sample;

      /* Output to left output channel */       
      while (!DSK6713_AIC23_write(hCodec, (Uint32) left_output));

      if ( 0 == switch_value)
        {
         left_output = left_input;
        }
      else if ( 1 == switch_value)
        {
          /* First order low pass filter */
          left_output = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], left_input);
        }   
      else if ( 2 == switch_value)
        {
        /* Second order low pass filter */
         left_output = second_order_IIR_direct_form_I ( &IIR_low_pass_2000Hz[0], left_input);
        }
      else if ( 3 == switch_value)
        {
         /* Create 3rd order filter by cascading first order and second order filters */
          temp = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], left_input);
          left_output = second_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], temp);
        }  
      else if ( 4 == switch_value)
        {
          /* Fourth order low pass filter */
          left_output = fourth_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], left_input);
        }
      else if ( 5 == switch_value)
        {
         /* Create 5th order filter by cascading first order and fourth order filters */
          temp = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], left_input);
          left_output = fourth_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], temp );
        }  
      else if ( 6 == switch_value)
        {
         /* First order high pass filter */
          left_output = first_order_IIR_direct_form_I ( &first_order_high_pass_2000Hz[0], left_input);
        }        
      else if ( 7 == switch_value)
        {
         /* Second order high pass filter */
         left_output = second_order_IIR_direct_form_I( &IIR_high_pass_2000Hz[0], left_input);
        }
      else if ( 8 == switch_value)
        {  
         /* Third order high pass filter */
         temp = first_order_IIR_direct_form_I ( &first_order_high_pass_2000Hz[0], left_input);
         left_output = second_order_IIR_direct_form_I (&IIR_high_pass_2000Hz[0], temp);
        }
      else if ( 9 == switch_value)
        {
         /* Fourth order high pass filter */
         left_output = fourth_order_IIR_direct_form_I( &IIR_high_pass_2000Hz[0], left_input);
        }  
      else if ( 10 == switch_value)
        {
         /* Fifth order high pass filter */
         temp = first_order_IIR_direct_form_I ( &first_order_high_pass_2000Hz[0], left_input);
         left_output = fourth_order_IIR_direct_form_I (&IIR_high_pass_2000Hz[0], temp);
        }
      else if ( 11 == switch_value)
        {
         /* First order combined high pass and low pass filter */
         left_output = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], mono_input);
        }  
      else if ( 12 == switch_value)
        {
         /* Second order combined high pass and low pass filter */
         left_output = second_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], mono_input);
        }
      else if ( 13 == switch_value)
        {
         /* Third order combined high pass and low pass filter */ 
         temp = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], mono_input);
         left_output = second_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], temp);
        }  
      else if ( 14 == switch_value)
        {
         /* Fourth order combined high pass and low pass filter */
         left_output = fourth_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], mono_input);
        }         
      else if ( 15 == switch_value)
        {
         /* Fifth order combined high pass and low pass filters */
         temp = first_order_IIR_direct_form_I ( &first_order_low_pass_2000Hz[0], mono_input);
         left_output = fourth_order_IIR_direct_form_I( &IIR_low_pass_2000Hz[0], temp);
        }

      /* Read right input channel */
      while (!DSK6713_AIC23_read(hCodec, &sample));
      right_input = (Int16) sample;

      /* Output to right output channel */
      while (!DSK6713_AIC23_write(hCodec, (Uint32) right_output));
      
      /* Read user switches on DSK and display their meaning on Stdout. */
      switch_value = switch_status_display();
	  
	  /* Where required, generate mono input from left input and right input */
      mono_input = stereo_to_mono ( left_input, right_input);

      if ( 0 == switch_value)
        {
         right_output = right_input;
        }
      else if ( 1 == switch_value)
        {
          /* First order low pass filter */
          right_output = first_order_IIR_direct_form_II (&first_order_low_pass_2000Hz[0], right_input);
        }   
      else if ( 2 == switch_value)
        {
          /* Second order low pass filter */
          right_output = second_order_IIR_direct_form_II (&IIR_low_pass_2000Hz[0], right_input);
        }
      else if ( 3 == switch_value)
        {
          /* Create 3rd order filter by cascading first order and second order filters */
          temp = first_order_IIR_direct_form_II (&first_order_low_pass_2000Hz[0], right_input);
          right_output = second_order_IIR_direct_form_II (&IIR_low_pass_2000Hz[0], temp);
        }  
      else if ( 4 == switch_value)
        {
          /* Fourth order low pass filter */
          right_output = fourth_order_IIR_direct_form_II( &IIR_low_pass_2000Hz[0], right_input);
        }
      else if ( 5 == switch_value)
        {
         /* Create 5th order filter by cascading first order and fourth order filters */
          temp = first_order_IIR_direct_form_II (&first_order_low_pass_2000Hz[0], right_input);
          right_output = fourth_order_IIR_direct_form_II( &IIR_low_pass_2000Hz[0], temp );
         }  
      else if ( 6 == switch_value)
        {
          /* First order high pass filter */
          right_output = first_order_IIR_direct_form_II (&first_order_high_pass_2000Hz[0], right_input);
        }        
      else if ( 7 == switch_value)
        {
         /* Second order high pass filter */
         right_output = second_order_IIR_direct_form_II (&IIR_high_pass_2000Hz[0], right_input);
        }
      else if ( 8 == switch_value)
        {  
         /* Third order high pass filter */
         temp = first_order_IIR_direct_form_II (&first_order_high_pass_2000Hz[0], right_input);
         right_output = second_order_IIR_direct_form_II (&IIR_high_pass_2000Hz[0], temp);
        }
      else if ( 9 == switch_value)
        {
         /* Fourth order high pass filter */
         right_output = fourth_order_IIR_direct_form_II( &IIR_high_pass_2000Hz[0], right_input);
        }  
      else if ( 10 == switch_value)
        {
         /* Fifth order high pass filter */
         temp = first_order_IIR_direct_form_II (&first_order_high_pass_2000Hz[0], right_input);
         right_output = fourth_order_IIR_direct_form_II (&IIR_high_pass_2000Hz[0], temp);
        }
      else if ( 11 == switch_value)
        {
         /* First order combined high pass and low pass filter */
         right_output = first_order_IIR_direct_form_II (&first_order_high_pass_2000Hz[0], mono_input);
        }  
      else if ( 12 == switch_value)
        {
         /* Second order combined high pass and low pass filter */
         right_output = second_order_IIR_direct_form_II (&IIR_high_pass_2000Hz[0], mono_input);
        }
      else if ( 13 == switch_value)
        {
         /* Third order combined high pass and low pass filter */ 
         temp = first_order_IIR_direct_form_II ( &first_order_high_pass_2000Hz[0], mono_input);
         right_output = second_order_IIR_direct_form_II (&IIR_high_pass_2000Hz[0], temp);       
        }  
      else if ( 14 == switch_value)
        {
         /* Fourth order combined high pass and low pass filter */
         right_output = fourth_order_IIR_direct_form_II( &IIR_high_pass_2000Hz[0], mono_input);
        }         
      else if ( 15 == switch_value)
        {
         /* Fifth order combined high pass and low pass filters */
         temp = first_order_IIR_direct_form_II ( &first_order_high_pass_2000Hz[0], mono_input);
         right_output = fourth_order_IIR_direct_form_II( &IIR_high_pass_2000Hz[0], temp);         
        }
  
      /* Display greater of two outputs on bargraph */
      bargraph_6dB( left_output, right_output);
     }

    /* Finished processing. Close the codec */
    DSK6713_AIC23_closeCodec(hCodec);
    
    puts("TMS320C6713 DSK has terminated\n");
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
/* End of IIR_order.c                                                         */
/******************************************************************************/

