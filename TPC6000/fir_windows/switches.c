/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	switches.c                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Read the four DIP user input switches on TMS320C6713 DSK and convert    */
/*   to a number in the range 0-15.                                          */
/*   Display meaning of switch setting on Stdout.                            */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Fouad El Khiati                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision 1.10                                                           */
/*   23rd October 2026. Modified for 48000 Hz operation.                     */
/*   Revision 1.00                                                           */
/*   23rd October 2026. Created by Richard Sikora from TMS320C5416 DSK code. */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>    /* Required for functions printf() and puts()          */

/*****************************************************************************/
/*                                                                           */
/* The name of the following header file will change if used as part of a    */
/* different project.  New project xxxx will use xxxxcfg.h instead.          */
/*                                                                           */
/*****************************************************************************/


#include "dsk6713.h"
#include "dsk6713_dip.h"

/*****************************************************************************/
/* user_switches_read()                                                      */
/*---------------------------------------------------------------------------*/
/* Read switch values from CPLD.                                             */
/* The value in range 0 - 15 is then debounced in switch_status_display()    */
/*                                                                           */
/*****************************************************************************/

static unsigned int user_switches_read(void)
{
  unsigned int temp;

  temp = 0;
     
  if ( DSK6713_DIP_get(0) )
    {
     /* Switch 0 is on */
     temp = 0x0001; 
    }
     
  if ( DSK6713_DIP_get(1) )
    {
     /* Switch 1 is on */
     temp |= 0x0002;
    }

  if ( DSK6713_DIP_get(2) )
    {
     /* Switch 2 is on */
     temp |= 0x0004;
    }
       
  if ( DSK6713_DIP_get(3) )
    {
     /* Switch 3 is on */
     temp |= 0x0008;
    }       

  /* Return value built up in temp */
       
  return( temp );                                                  
}


/*****************************************************************************/
/* switch_status_display().                                                  */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Read switch value every tenth of a second.                                */
/* Display meaning of switch on Stdout.                                      */
/*                                                                           */
/*****************************************************************************/

unsigned int switch_status_display(void)
{
 static unsigned int last_switch_values[2] = {99,99};
 static unsigned int current_switch_value = 0;
 static unsigned int counter = 0;
 unsigned int temp;

 /* The variable 'counter' reaches zero 10 times per second. */
 /* Check the switch reading.                                */
   
 if ( 0 == counter) 
   {         
     /* Read latest switch settings. */
   
     temp = user_switches_read(); 
     
     /* Test if switch reading is the same as the last one. */
     
     if ( temp == last_switch_values[0] )
       {
         /* Same reading as last time. Two identical values received */
         current_switch_value = temp; /* Use new value */
         
         /* Only update display on Stdout when switch has just changed */
         
         if ( temp != last_switch_values[1] )
           {
            /* Reading at switches has changed. */
            /* Display new switch value.        */              
                        
            if ( 0 == temp )
               { 
                 puts("User switches = 0. Straight through, no FIR filter.\n"); 
               }                                                 
            else if ( 1 == temp)
               {
                 puts("User switches = 1. 1200 Hz high pass FIR filter using rectangular window.\n");
               }                                                   
            else if ( 2 == temp )
               {
                 puts("User switches = 2. 1200 Hz high pass FIR filter using Hamming window.\n");
               }                                                  
            else if ( 3 == temp )
               {
                 puts("User switches = 3. 1200 Hz high pass FIR filter using Hanning window.\n");
               }  
            else if ( 4 == temp )
               { 
                 puts("User switches = 4. 1200 Hz high pass FIR filter using Blackman window.\n"); 
               }                
            else if ( 5 == temp )
               {
                 puts ("User switches = 5. 1200 Hz high pass FIR filter using Kaiser window.\n");
               }                                    
            else if ( 6 == temp)
               {
                 puts("User switches = 6. 1200 Hz low pass FIR filter using rectangular window.\n");
               }                                                   
            else if ( 7 == temp )
               {
                 puts("User switches = 7. 1200 Hz low pass FIR filter using Hamming window.\n");
               }                                                  
            else if ( 8 == temp )
               {
                 puts("User switches = 8. 1200 Hz low pass FIR filter using Hanning window.\n");
               }
            else if ( 9 == temp)
               {
                 puts("User switches = 9. 1200 Hz low pass FIR filter using Blackman window.\n");
               }       
            else if ( 10 == temp)
               {
                 puts ("User switches = 10. 1200 Hz low pass FIR filter using Kaiser window.\n");   
               }                                                                                        
            else if ( 11 == temp )
               {
                 puts("User switches = 11. Passe bas sur une voie, Passe haut sur l'autre\n");
               }                                                  
            else if ( 12 == temp )
               {
                 puts("User switches = 12. Libre, pas de filtrage pour l'instant. \n");
               }  
            else if ( 13 == temp )
               { 
                 puts("User switches = 13. Libre, pas de filtrage pour l'instant.\n"); 
               }                                                 
            else if ( 14 == temp)
               {
                 puts("User switches = 14. Libre, pas de filtrage pour l'instant.\n");
               }                                                   
            else if ( 15 == temp )
               {
                 puts("User switches = 15. Libre, pas de filtrage pour l'instant.\n");
               }                                                  
            else 
               {
                 /* User switches outside the allowed range 0 - 15 */
                 puts ("User switches out of range.\n");
               }    
           } 
       }       
       
     /* Shuffle switch readings along one place ready for next time. */  
     /* Then read in current switch status.                          */
  
     last_switch_values[1] = last_switch_values[0]; 
     last_switch_values[0] = temp;             
     } 
    
  /* Increment counter. If at maximum or over-range, go back to zero. */    
 
  if ( counter < 4800)
   {
    counter++;
   } 
  else
   {          
    /* Counter is at maximum value of 4800 or out of range. */
    /* Go back to the beginning.                            */
   
    counter = 0;
   } 
     
 return(current_switch_value);
}


/******************************************************************************/
/* End of switches.c                                                          */
/******************************************************************************/
