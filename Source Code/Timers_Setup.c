/*******************************************************************************
* Title: Timers_Setup.c                                                        
* Version: 1.0                                                                
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                         
*                                                                              
* Description:                                                                 
* This file contains all the functions that are needed in order to initialize  
* and control the operation of Timer0, Timer1 and Timer2.                      
*******************************************************************************/

#ifndef TIMERS_SETUP_C
#define TIMERS_SETUP_C

#include "Main_Includes.h"
#include "Timers_Setup.h"

/*******************************************************************************
* Function: TMR1_Init(void)                                           
*                                                                              
* Variables:                                                                   
* setup -> Stores the setup values for Timer1                                  
*                                                                              
* Description:                                                                 
* This function will initialize Timer1 as specified by the user.               
*******************************************************************************/
void TMR1_Init(void)
{ 
  //Set priority to 6 (2nd highest), clear interrupt flag and enable interrupt
  IPC0bits.T1IP = 6;	 
  IFS0bits.T1IF = 0;	 
  IEC0bits.T1IE = 1;	
  
  //Prescaler -> 1:256, Internal clock, Interrupt period -> 8192us
  PR1 = 2240;
  T1CON = 0x8030;  
}

/*******************************************************************************
* Function: TMR2_Init(void)                                
*                                                                              
* Variables:                                                                   
* N/A                        
*                                                                              
* Description:                                                                 
* This function will initialize Timer2 as specified by the user.               
*******************************************************************************/
void TMR2_Init(void)
{  
  PR2 = 159;         //Set PWM up for 250kHz Period
  T2CON = 0x8000;   //Enable TMR2 with no Prescaler
}

/*******************************************************************************
* Function: TMR3_Init(void)                                           
*                                                                              
* Variables:                                                                                                   
* N/A                                         
*                                                                              
* Description:                                                                 
* This function will initialize Timer3 as specified by the user.               
*******************************************************************************/
void TMR3_Init(void)
{
  //Set priority to 5 (3rd highest), clear interrupt flag and enable interrupt
  IPC2bits.T3IP = 5;	 
  IFS0bits.T3IF = 0;	 
  IEC0bits.T3IE = 1;
  
  //Prescaler -> 1:8, Internal clock, Interrupt period -> 1ms
  PR3 = 8750;
  T3CON = 0x8010;  	 
}

/*******************************************************************************
* Function: TMR3_Init(void)                                           
*                                                                              
* Variables:                                                                                                   
* N/A                                         
*                                                                              
* Description:                                                                 
* This function will initialize Timer3 as specified by the user.               
*******************************************************************************/
void TMR4_Init(void)
{
  //Set priority to 4 (4th highest), clear interrupt flag and enable interrupt
  _T4IP = 4;	 
  _T4IF = 0;	 
  _T4IE = 1;
  
  //Prescaler -> 1:8, Internal clock, Interrupt period -> 1ms
  PR4 = 65535;
  T4CON = 0x8030;  	 
}

/*******************************************************************************
* Function: TMR5_Init(void)                                           
*                                                                              
* Variables:                                                                                                   
* N/A                                         
*                                                                              
* Description:                                                                 
* This function will initialize Timer5 as specified by the user.               
*******************************************************************************/
void TMR5_Init(void)
{
  //Set priority to 7 (Highest), clear interrupt flag and enable interrupt  
  _T5IP = 7;	 //set interrupt priority
  _T5IF = 0;	 //reset interrupt flag
  _T5IE = 1;	 //turn on the timer5 interrupt   
  
  //Prescaler -> 1:1, Internal clock, Interrupt period -> 1ms
  PR5 = 8750;
  T5CON = 0x8010; 
  
}

#endif
