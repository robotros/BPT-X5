/*******************************************************************************
* Title: 74HC595_Setup.h                                                       
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that setup the 74HC595 8-Bit Shift Register 
* and send data to it via the Serial Peripheral Interface. (SPI)               
*******************************************************************************/

#ifndef _74HC595_SETUP_H
#define _74HC595_SETUP_H   

/*************************************************
*               Bit Definitions                  *
*************************************************/
//74HC595 Control Lines
#define HC595_RST              _LATD6
#define HC595_LAT              _LATD5
#define HC595_EN               _LATA7

//74HC595 Data & Clock Line; SPI pins
#define HC595_SIN              _LATA4
#define HC595_CLK              _LATC3

/*************************************************
*                  Constants                     *
*************************************************/ 
//Amount of 74HC595 chips cascaded together
#define HC595_CHIPS            6

//Set this definition to your specfified SPI module
#define HC595_SPI_Send(x)      SPI1_Send(x)

/*************************************************
*              Function Prototypes               *
*************************************************/   
void HC595_Init(void);   
void HC595_Reset(void);                        
void HC595_Send_Data(UINT8 *data);

#endif