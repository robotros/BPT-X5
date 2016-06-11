/*******************************************************************************
* Title: SPI_Setup.c                                                            
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                        
*                                                                               
* Description:                                                                   
* This file contains the functions that allow the user to set up the SPI and    
* send and receive data.                                                        
*******************************************************************************/
            
#ifndef SPI_SETUP_C
#define SPI_SETUP_C

#include "Main_Includes.h"
#include "SPI_Setup.h"

/*******************************************************************************
* Function: SPI1_Init(void)                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                  
* This function will initialize the SPI module according to the parameters set by
* the user.               
*******************************************************************************/
void SPI1_Init(void)
{ 
  SPI1CON1 = 0x0135;
  SPI1CON2 = 0x0000;
  SPI1STAT = 0x8000;
  SPI1_STATE(1);
}

/*******************************************************************************
* Function: SPI2_Init(void)                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will initialize the SPI module according to the paramters set by
* the user.          
*******************************************************************************/
void SPI2_Init(void)
{
  //Internal clock enabled, change on active to idle, master mode, PPRE -> 16:1
  //SPRE -> 3:1 ; CLK_SPEED = 40MHz / (16 * 3) = 833.33KHz 
  //SPI2CON1 = 0x013F; - 74HC595
  SPI2CON1 = 0x0135;
  SPI2CON2 = 0x0000;
  SPI2STAT = 0x0000;
  
  SPI2_STATE(1);
}

/*******************************************************************************
* Function: SPI1_Send(byte tx)                                                  
*                                                                              
* Variables:                                                                   
* tx -> The value of the data that is to be transmitted                        
*                                                                              
* Description:                                                                 
* This function will send the value of tx and return the received data.                
*******************************************************************************/
UINT16 SPI1_Send(UINT16 tx) 
{
  //Begin sending the data
  SPI1BUF = tx;
  
  //Wait for data to finish sending
  while(!SPI1STATbits.SPIRBF);
  
  //Return received data
  return SPI1BUF;
}

/*******************************************************************************
* Function: SPI2_Send(byte Tx)                                                 
*                                                                              
* Variables:                                                                   
* tx -> The value of the data that is to be transmitted                        
*                                                                              
* Description:                                                      
* This function will send the value of tx and return the received data.                
*******************************************************************************/
UINT16 SPI2_Send(UINT16 tx) 
{
  //Begin sending the data
  SPI2BUF = tx;
  
  //Wait for data to finish sending
  while(!SPI2STATbits.SPIRBF);
  
  //Return received data
  return SPI2BUF;
}

#endif
