/*******************************************************************************
* Title: Interrupts.h                                                           
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                          
*                                                                               
* Description:                                                                  
* This file stores the initialization of certain interrupt peripherals. Currently,
* it is only used to set up the External Interrupt #0 which is used for the IR  
* receiver.                                                                     
*******************************************************************************/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/*************************************************
*                   Constants                    *
*************************************************/
//Used in INT0_Init(a,b,c) to set the interrupt edge
#define INT0_NEGATIVE_EDGE						0x0001
#define INT0_POSITIVE_EDGE						0x0000

//Used in INT1_Init(a,b,c) to set the interrupt edge
#define INT1_NEGATIVE_EDGE						0x0002
#define INT1_POSITIVE_EDGE						0x0000

//Used in INT2_Init(a,b,c) to set the interrupt edge
#define INT2_NEGATIVE_EDGE						0x0004
#define INT2_POSITIVE_EDGE						0x0000

//Used to set the priority of the specified interrupt
#define INT_PRIORITY0									0x0000
#define INT_PRIORITY1									0x0001
#define INT_PRIORITY2									0x0002
#define INT_PRIORITY3									0x0003
#define INT_PRIORITY4									0x0004
#define INT_PRIORITY5									0x0005
#define INT_PRIORITY6									0x0006
#define INT_PRIORITY7									0x0007

/*************************************************
*                   Macros                       *
*************************************************/
//Global Interrupt Control Handlers
#define ENABLE_GLOBAL_INTERRUPTS()		(INTCON2 |= 0x8000)
#define DISABLE_GLOBAL_INTERRUPTS()		(INTCON2 &= 0x7FFF)

/*************************************************
*              Function Prototypes               *
*************************************************/
void INT0_Init(UINT16 state, UINT16 polarity, UINT16 priority);
void INT1_Init(UINT16 state, UINT16 polarity, UINT16 priority);
void INT2_Init(UINT16 state, UINT16 polarity, UINT16 priority);

#endif
