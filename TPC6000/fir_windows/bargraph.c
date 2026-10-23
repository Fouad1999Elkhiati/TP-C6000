/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 bargraph.c                                                              */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C6713 DSK.                                                        */
/*   Configures four user LEDs as a bargraph display.                        */
/*   Different formats of display for mono audio, stereo audio or power      */
/*   for use with Fast Fourier Transform (FFT).                              */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*                                                                           */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora from TMS320C5402 DSK code. */
/*                                                                           */
/*****************************************************************************/


#include "dsk6713.h"
#include "dsk6713_led.h"


/*****************************************************************************/
/* Reference thresholds                                                      */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* 0 dB reference is taken to be 50% output level. This allows 6dB headroom  */
/* Other thresholds are -6dB, -12dB and -18dB below the reference of 0dB.    */
/*                                                                           */ 
/*****************************************************************************/

#define LEVEL_0dB  0x4000
#define LEVEL_3dB  0x2D3F
#define LEVEL_6dB  0x2000
#define LEVEL_9dB  0x169F
#define LEVEL_12dB 0x1000
#define LEVEL_18dB 0x0800
#define LEVEL_24dB 0x0400
#define LEVEL_36dB 0x0100


/*****************************************************************************/
/* bargraph_3dB()                                                            */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph using 3dB increments.        */
/* For mono use, use the single mono input for both the left and the right   */
/* channels.                                                                 */
/*                                                                           */
/*****************************************************************************/

void bargraph_3dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( -32768 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( -32768 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    DSK6713_LED_on(0);
    DSK6713_LED_on(1);
	DSK6713_LED_on(2);
    DSK6713_LED_on(3);
  }
 else if ( input > LEVEL_3dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_on(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }           
 else if ( input > LEVEL_6dB)
  {
    /* Turn on 2 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }  
 else if ( input > LEVEL_9dB)
  { 
    /* Turn on 1 LED */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_on(3);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_off(3);
  }   

}

/*****************************************************************************/
/* bargraph_6dB()                                                            */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph. For mono use, use the       */
/* single mono input for both the left and the right channels.               */
/*                                                                           */
/*****************************************************************************/

void bargraph_6dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( -32768 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( -32768 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    DSK6713_LED_on(0);
    DSK6713_LED_on(1);
	DSK6713_LED_on(2);
    DSK6713_LED_on(3);
  }
 else if ( input > LEVEL_6dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_on(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }           
 else if ( input > LEVEL_12dB)
  {
    /* Turn on 2 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }  
 else if ( input > LEVEL_18dB)
  { 
    /* Turn on 1 LED */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_on(3);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_off(3);
  }   

}


/*****************************************************************************/
/* bargraph_12dB()                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph using 3dB increments.        */
/* For mono use, use the single mono input for both the left and the right   */
/* channels.                                                                 */
/*                                                                           */
/*****************************************************************************/

void bargraph_12dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( -32768 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( -32768 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    DSK6713_LED_on(0);
    DSK6713_LED_on(1);
	DSK6713_LED_on(2);
    DSK6713_LED_on(3);
  }
 else if ( input > LEVEL_12dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_on(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }           
 else if ( input > LEVEL_24dB)
  {
    /* Turn on 2 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_on(2);   
    DSK6713_LED_on(3);
  }  
 else if ( input > LEVEL_36dB)
  { 
    /* Turn on 1 LED */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_on(3);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    DSK6713_LED_off(0);
    DSK6713_LED_off(1); 
    DSK6713_LED_off(2);   
    DSK6713_LED_off(3);
  }   

}

/*****************************************************************************/
/* spectrum_analyser_display()                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Each input represents the power in a particular frequency band, and       */
/* therefore can only be positive.                                           */
/*                                                                           */
/*****************************************************************************/

void spectrum_analyser_display( unsigned int input1,
                                unsigned int input2, 
                                unsigned int input3,
                                unsigned int input4 )
{
  /* Display each input on a different LED */
  
 if ( input1 > LEVEL_0dB)
  {
    DSK6713_LED_on(3);
  } 
 else 
  {
    DSK6713_LED_off(3);
  } 
  
 if ( input2 > LEVEL_0dB)
  {
    DSK6713_LED_on(2); 
  } 
 else 
  {
    DSK6713_LED_off(2);
  }                           
  
 if ( input3 > LEVEL_0dB)
  {
    DSK6713_LED_on(1);
  } 
 else 
  {
    DSK6713_LED_off(1);
  } 
    
 if ( input4 > LEVEL_0dB)
  {
    DSK6713_LED_on(0);
  } 
 else 
  {
    DSK6713_LED_off(0);
  }   
 
}
 
/*****************************************************************************/
/* End of bargraph.c                                                         */
/*****************************************************************************/
