/*******************************************************************************
* Title: ADC_Setup.c                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains all the functions that are needed in order to initialize  
* and control the operation of the ADC module.                                 
*******************************************************************************/

#ifndef ADC_SETUP_C
#define ADC_SETUP_C

#include "Main_Includes.h"
#include "ADC_Setup.h"
#include "Delay_Setup.h"

/*******************************************************************************
* Function: ADC_Init(void)                                              
*                                                                              
* Variables:                                                                   
* setup -> Stores the setup values of the ADC module                           
*                                                                              
* Description:                                                                 
* This function will initialize the ADC module according to the specifications 
* that the user has set.                                                       
*******************************************************************************/
void ADC_Init(void)
{    
	//Set AN0 - AN3 as analog inputs
  ANSELA = 0x0003;
  ANSELB = 0x0003;

  //Turn on, auto sample start, auto-convert
  AD1CON1 = 0x000C;

  //Vref+, Vref-, int every conversion, MUXA only
  AD1CON2 = 0x0300;

  //31 Tad auto-sample, Tad = 5*Tcy
  AD1CON3 = 0x0005;
  
  //Allocate two WORDs in buffer
  AD1CON4 = 8;

  //No sequential scanning
  AD1CSSL = 0;
  
  //Choose channel
  AD1CHS0 = 3;  
  
  // Select AN3 for CH0 +ve input
  AD1CHS0bits.CH0SA = 3; 
  
  //Set input channels
  AD1CHS123 =0;
  
  _ADON = 1;
}

/*******************************************************************************
* Function: ADC_Read(void)                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will read the ADC value on whichever ANx Channel is currently 
* selected. It will then return the 10-Bit value that was read on the ADC pin as
* a 16-Bit variable.                                
*******************************************************************************/
UINT16 ADC_Read(void)
{
  //Ensure that the DONE bit is cleared
  _DONE = 0;
  
  //Begin ADC sample
  _SAMP = 1;
  
  //Wait for AD Conversion to finish
  while (!_DONE);
  
  //Return the ADC value
  return ADC1BUF0; 
}

/*******************************************************************************
* Function: ADC_Read(void)                                                     
*                                                                              
* Variables:                                                                   
* *buf -> Pointer to an array of four UINT16 variables that will store the ADC values                                                                          
*                                                                              
* Description:                                                                 
* This function will read the ADC value on whichever four ADC channels s=
* simultaneously (AN0 - AN3). The data                                
*******************************************************************************/
void ADC_Read_SS(UINT16 *buf)
{
  //Begin ADC sample
  _SAMP = 0;
  
  //Wait for AD Conversion to finish
  while (!_AD1IF);
  
  // Clear conversion done status bit
  _AD1IF = 0;
  
  //Save each ADC reading. AN0 -> buf[1], AN1 -> buf[1], AN2 -> buf[2], AN3 -> buf[3]
  buf[0] = ADC1BUF1; 
  buf[1] = ADC1BUF2; 
  buf[2] = ADC1BUF3; 
  buf[3] = ADC1BUF0; 
}

#endif
