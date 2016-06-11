/*******************************************************************************
* Title: Timer_Setup.h                                                          
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                         
*                                                                               
* Description:                                                                  
* This file contains all the constants and function prototypes that are used    
* to initialize and control the operation of Timer0 and Timer1.                 
*******************************************************************************/

#ifndef TIMERS_SETUP_H
#define TIMERS_SETUP_H

/*************************************************
*                  Constants                     *
*************************************************/    

/*************************************************
*                   Macros                       *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/  
void TMR1_Init(void);   
void TMR2_Init(void);       
void TMR3_Init(void);     
void TMR5_Init(void);

#endif
