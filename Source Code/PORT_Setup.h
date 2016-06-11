/*******************************************************************************
* Title: PORT_Setup.h                                                           
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: January 10, 2010                                                        
*                                                                               
* Description:                                                                  
* This file holds the function prototype(s) for initializing the PORTs, various
* peripherals and setting up the oscillator.
*******************************************************************************/

#ifndef PORT_SETUP_H
#define PORT_SETUP_H

/*************************************************
*              Function Prototypes               *
*************************************************/   
void PORT_Init(void);  
void Modules_Init(void);
void Oscillator_Init(void);

#endif                 
