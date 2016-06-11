/*******************************************************************************
* Title: Delay_Setup.c                                                         
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: April 23, 2014                                                       
*                                                                              
* Description:                                                                 
* This library contains various timing routines, one that uses an interrupt, the 
* others are simple delays which will halt the program for the desired amount of
* time.                                                                        
*******************************************************************************/

#ifndef DELAY_SETUP_H
#define DELAY_SETUP_H

#include "Main_Includes.h"
#include "Delay_Setup.h"

//Must be declared in global variables
extern volatile UINT32 count32;

/*******************************************************************************
* Function: Time_Check(UINT32 *mark, UINT16 interval)                           
*                                                                              
* Variables:                                                                   
* mark - >                                                                     
* interval ->                                                                  
*                                                                              
* Description:                                                                 
* This function is used in accordance with a timer. 'count32' is a global variable
* which increments each time it passes through a user specified timer interrupt.
* A static variable must be declared in the function that uses the routine, the static          
* variable will be pointed to with *mark. *mark stores the original count32 value from
* when the function was called. The interval is the amount of interrupts that 
* need to elapse from (count32 + interval). By subtracting the current value of count32                                    
* from *mark we can compare to see if we have surpassed the value of interval, thus 
* returning a (1) and letting us know that the required time has elapsed. If a (0) is
* returned then the program can go on to other functions in the main loop before 
* coming back and checking the time interval once again.                                                                             
*******************************************************************************/
UINT8 Time_Check(UINT32 *mark, UINT16 interval) 
{
  //Check to see if the required time interval has elapsed
  if ((count32 - *mark) >= interval)
  {
    //If the required interval has elapsed, reset *mark to count32's current value
    //Return a 1 to let the program know that the time has elapsed
    *mark = count32;
    return 1;  
  }  
  
  //The required time has not elapsed, return a 0
  return 0;
}

#endif
