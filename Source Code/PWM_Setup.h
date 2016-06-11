/*******************************************************************************
* Title: PWM_Setup.h                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that allow the user to setup the PWM modules
* and adjust the duty cycle and period of the PWM module.                      
*******************************************************************************/

#ifndef PWM_SETUP_H
#define PWM_SETUP_H

/*************************************************
*                  Constants                     *
*************************************************/ 

/*************************************************
*                   Macros                       *
*************************************************/
#define PWM2_DUTY(x)        (OC2RS = x)

/*************************************************
*              Function Prototypes               *
*************************************************/  
void PWM1_Init(UINT16 timebase, UINT16 duty);
void PWM2_Init(UINT16 timebase, UINT16 duty);       

#endif
