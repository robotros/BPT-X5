/*******************************************************************************
* Title: Interrupts.c                                                           
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                          
*                                                                               
* Description:                                                                  
* This file stores the initialization of certain interrupt peripherals. Currently,
* it is only used to set up the External Interrupts.                                                          
*******************************************************************************/

#ifndef INTERRUPTS_C
#define INTERRUPTS_C

#include "Main_Includes.h"
#include "Interrupts.h"

/*******************************************************************************
* Function: INT0_Init(void)                                                      
*                                                                               
* Variables:                                                                    
* state -> Controls whether the module is enabled (1) or disabled (0)
* polarity -> Interrupt on negative edge (1) or positive edge (0)
* priority -> Sets the priority of the interrupt                                                                           
*                                                                               
* Description:                                                                  
* This function will set up the external interrupt #0 module for operation. The
* user has to choose to enable/disable it, set the polarity of the interrupt, so
* whether it interrupts on the negative edge of a signal or the positive edge, and
* the priority of the interrupt, which will be between 0 - 7.                                                                              
*******************************************************************************/
void INT0_Init(UINT16 state, UINT16 polarity, UINT16 priority)
{ 
	//Set the interrupt to trigger on a specific type of signal	
  INTCON2 |= polarity;
  
  //Set the interrupt priority for the interrupt
  _INT0IP = priority;
  
  //Make sure that the interrupt flag is cleared
  _INT0IF = 0;
  
  //Enable or disable ther interrupt, depending on the value of 'state'
  _INT0IE = state; 
}

/*******************************************************************************
* Function: INT1_Init(void)                                                      
*                                                                               
* Variables:                                                                    
* state -> Controls whether the module is enabled (1) or disabled (0)
* polarity -> Interrupt on negative edge (1) or positive edge (0)
* priority -> Sets the priority of the interrupt                                                                           
*                                                                               
* Description:                                                                  
* This function will set up the external interrupt #1 module for operation. The
* user has to choose to enable/disable it, set the polarity of the interrupt, so
* whether it interrupts on the negative edge of a signal or the positive edge, and
* the priority of the interrupt, which will be between 0 - 7.                                                                              
*******************************************************************************/
void INT1_Init(UINT16 state, UINT16 polarity, UINT16 priority)
{ 
	//Set the interrupt to trigger on a specific type of signal	
  INTCON2 |= polarity;
  
  //Set the interrupt priority for the interrupt
  _INT1IP = priority;
  
  //Make sure that the interrupt flag is cleared
  _INT1IF = 0;
  
  //Enable or disable ther interrupt, depending on the value of 'state'
  _INT1IE = state; 
}

/*******************************************************************************
* Function: INT2_Init(void)                                                      
*                                                                               
* Variables:                                                                    
* state -> Controls whether the module is enabled (1) or disabled (0)
* polarity -> Interrupt on negative edge (1) or positive edge (0)
* priority -> Sets the priority of the interrupt                                                                           
*                                                                               
* Description:                                                                  
* This function will set up the external interrupt #2 module for operation. The
* user has to choose to enable/disable it, set the polarity of the interrupt, so
* whether it interrupts on the negative edge of a signal or the positive edge, and
* the priority of the interrupt, which will be between 0 - 7.                                                                              
*******************************************************************************/
void INT2_Init(UINT16 state, UINT16 polarity, UINT16 priority)
{ 
	//Set the interrupt to trigger on a specific type of signal	
  INTCON2 |= polarity;
  
  //Set the interrupt priority for the interrupt
  _INT2IP = priority;
  
  //Make sure that the interrupt flag is cleared
  _INT2IF = 0;
  
  //Enable or disable ther interrupt, depending on the value of 'state'
  _INT2IE = state; 
}
  
#endif
