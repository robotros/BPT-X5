/*******************************************************************************
* Title: MSGEQ7_Setup.h                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the function prototypes and definitions that are used to                                                                              
* control the operation of the MSGEQ7 and the VU Meter Attachment.                                                                                  
*******************************************************************************/
   
#ifndef MSGEQ7_SETUP_H
#define MSGEQ7_SETUP_H
 
/*************************************************
*                   Constants                    *
*************************************************/
#define MSG_STROBE            PORTAbits.RA12
#define MSG_RESET             PORTAbits.RA11
#define MSG_IN                PORTBbits.RB1

//These values add an offset for each channel; Used to filter out noise	
//If you have any channels that are getting high values even with no sound,
//raise the offset amount here
#define CH0_OFFSET						50
#define CH1_OFFSET						50
#define CH2_OFFSET						80
#define CH3_OFFSET						80
#define CH4_OFFSET						80
#define CH5_OFFSET						100
#define CH6_OFFSET						100

//This determines the resolution of the returned VU signals
#define VU_STEPS							32

//Enables peak hold for the VU Meter. 1 = Enable; 0 = Disable
#define PEAK_HOLD							1
#define PEAK_LEVEL						1

/*************************************************
*              Function Prototypes               *
*************************************************/
void MSGEQ7_Init(void);
void MSGEQ7_Read(UINT16 *channel);
void MSGEQ7_Auto_Adjust(UINT16 *chan, UINT16 *level);

#endif