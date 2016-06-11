/*******************************************************************************
* Title: 74HC595_Setup.c                                                       
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that setup the 74HC595 8-Bit Shift Register 
* and send data to it via the Serial Peripheral Interface. (SPI)               
*******************************************************************************/

#ifndef _74HC595_SETUP_C
#define _74HC595_SETUP_C   

#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "SPI_Setup.h"
#include "74HC595_Setup.h"

/*******************************************************************************
* Function: HC595_Init(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will set the control lines for the 74HC595(s) to ensure that they                                                                             
* are ready for operation.                                                                             
*******************************************************************************/
void HC595_Init(void)
{
  //Reset each 74HC595
  HC595_Reset();
  
  //Set Latch high
  HC595_LAT = 1; 
  
  //Enable the shift registers
  HC595_EN = 0;
} 

/*******************************************************************************
* Function: HC595_Reset(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will reset each 74HC595 shift register.                       
*******************************************************************************/
void HC595_Reset(void)
{
  //Pulse the reset pin
  HC595_RST = 0;
  HC595_RST = 1; 
}  

/*******************************************************************************
* Function: HC595_Send_Data(UINT8 *data)                                                                   
*                                                                              
* Variables:                                                                   
* *data -> The buffered data that is to be sent to the 74HC595(s)                                                                         
*                                                                              
* Description:                                                                 
* This function will send a byte of data to each of the shift registers cascaded                                                                             
* together, then latch the data bits into their outputs.                                                                             
*******************************************************************************/
void HC595_Send_Data(UINT8 *data)
{
  UINT8 i;
 
  //Set latch idle
  HC595_LAT = 0;
  
  //Send a byte for each shift register
  for (i = 0;i < HC595_CHIPS;i++)
    HC595_SPI_Send(data[i]);
    
  //Latch in the data on a rising edge
  HC595_LAT = 1; 
}  

#endif
