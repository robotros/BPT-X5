/*******************************************************************************
* Title: TLC5955_Setup.c                                                       
* Version: 1.0                                                                    
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                       
*                                                                              
* Description:                                                                 
* This file contains the functions and routines that are used to                                                                             
* control the operation of the TLC5955 16-bit PWM driver.                                                                                                                            
*******************************************************************************/
   
#ifndef TLC5955_SETUP_C
#define TLC5955_SETUP_C  

#include "Main_Includes.h"
#include "PWM_Setup.h"
#include "Timers_Setup.h"
#include "TLC5955_Setup.h"
#include "SPI_Setup.h"
#include "PWM_Setup.h"
#include "Delay_Setup.h"

/*************************************************
*               Global Variables                 *
*************************************************/
extern T8_FLAG TLC;

extern T16_FLAG FLAG1;
extern volatile UINT16 TLC_data[96];
extern volatile UINT16 TLC_data2[96];

/*******************************************************************************
* Function: TLC5940_Init(void)                                                 
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will prepare the TLC5940 to start accepting data.              
*******************************************************************************/
void TLC5955_Init(UINT8 FC_data, UINT8 *DC_data, UINT16 MC_data, UINT32 BC_data)
{
  INT16 i,j;
  UINT8 data[96];
  
  //Allow the TLC5955 to receive data
  TLC_LAT = 0;
  TLC_ENA = 0;
  
  //Set the grayscale clock
  PWM1_Init(16,7);
  
  //Enable the timers that are used to control the fade routines and channel updates
  TMR1_Init();  //Enable Timer1
  TMR3_Init();  //Enable Timer3
  
  //Set max values for DC, BC, MC and FC controls
  for (i = 0;i < (TLC_CHIPS * 96);i++)
    data[i]= 0xFF;
  
  //Set the MSb[768:761] equal to 0x96 to enter control mode  
  data[95] = 0x96;  
   
  //Set up DC values (336 bits; 42 bytes)
  for (i = 0;i < 42;i++)
    data[i]= DC_data[i];
    
  //Pack the FC, BC and MC data
  data[42] = MC_data & 0x00FF;  
  data[43] = (BC_data << 1) | (MC_data >> 8);  
  data[44] = (BC_data >> 7) & 0x000000FF; 
  data[45] = (BC_data >> 14) | ((FC_data << 6) & 0xC0);
  data[46] = (FC_data & 0x1C) >> 2; 
  
  //Enable SPI
  SPI2_Init(); 
   
  //Now we write the exact same data into the chip again as
  //one write will not change the BC values from their default values. 
  //We have to do two consecutive writes to change the BC data values   
  for (j = 0;j < TLC_CHIPS;j++)
  {   
    //Set the TLC5955 into control mode  
    Write_Single_Bit(TLC5955_CONTROL_MODE);
    
    //Write the control data into the chip  
    for (i = 95;i >= 0;i--)
      SPI2_Send(data[i]);
  }    
  
  //Latch in new data into the Common Shift Register  
  PULSE(TLC_LAT);
}

/*******************************************************************************
* Function: TLC5940_Init(void)                                                 
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will prepare the TLC5940 to start accepting data. The current 
* output for each channel is set to a minimum (3.2mA p. 28 of datahsheet) as each
* channel connects through to a transistor/MOSFET.              
*******************************************************************************/
void TLC5955_Default_Init(UINT8 FC_data)
{
  INT16 i;
  UINT8 DC_data[42];
  UINT16 MC_data = 0x0000;
  UINT32 BC_data = 0x00000000;

  //Set max values for DC
  for (i = 0;i < 42;i++)
    DC_data[i]= 0x0;
  
  //Initalize the TLC5955 with its default value  
  TLC5955_Init(FC_data,DC_data,MC_data,BC_data);
}            
  
/*******************************************************************************
* Function: TLC5955_Update(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will move the channel data into a separate variable array, 
* effectively double buffering the data.                                                                             
*******************************************************************************/
void TLC5955_Update(void)
{
  UINT16 i;
  
  //Double buffer the current channel data
  for (i = 0;i < (TLC_CHIPS * 48);i++)
     TLC_data2[i] = TLC_data[i];   
  
  //Update the channels
  TLC5955_UPDATE = 1;  
}       

/*******************************************************************************
* Function: Write_Single_Bit(UINT8 state)                                                                   
*                                                                              
* Variables:                                                                   
* state -> Determines the state of the bit that is to be written                                                                          
*                                                                              
* Description:                                                                 
* This function will disable the SPI bus allowing the user to bit-bang a data
* bit into the TLC5955(s). This is useful as we have to send 769 bits of grayscale
* data to each TLC5955, but the SPI bus only sends data in chunks of 16-bits or
* 8-bits. This function is used to send the odd bit to the TLC5955 and then use
* SPI to send the rest of the data.                                                                            
*******************************************************************************/      
void Write_Single_Bit(UINT8 state)
{   
  //Disable SPI to send single bit  
  SPI2_STATE(0);
  
  //Set the state of the bit
  TLC_MOSI = state;
  
  //Shift in the bit with one clock pulse
  PULSE(TLC_SCK);
  
  //Re-enable SPI
  SPI2_STATE(1);
}    
  
/*******************************************************************************
* Function: TLC5955_Write_GS(UINT16 *data)                                                                    
*                                                                              
* Variables:                                                                   
* *data -> Points to the duty cycle values of each PWM channel                                                                         
*                                                                              
* Description:                                                                 
* This function is used to send new data into the TLC5955(s) and update the
* channels. This particular function is set up to control two TLC5955 drivers.                                                                             
*******************************************************************************/
void TLC5955_Write_GS(UINT16 *data)
{
  INT16 i;
  
  //Set the TLC5955 into GS mode so that we can update the PWM outputs  
  Write_Single_Bit(TLC5955_GS_MODE);
  
  //Write the GS data into the TLC5955
  for (i = 95;i >= 48;i--)
  {
    SPI2_Send(data[i] >> 8);
    SPI2_Send(data[i] & 0x00FF);
  }  
   
  //Set the TLC5955 into GS mode so that we can update the PWM outputs  
  Write_Single_Bit(TLC5955_GS_MODE);
  
  //Write the GS data into the TLC5955
  for (i = 47;i >= 0;i--)
  {
    SPI2_Send(data[i] >> 8);
    SPI2_Send(data[i] & 0x00FF);
  }  
  
  //Update the PWM outputs by latching the data  
  PULSE(TLC_LAT); 
}

#endif
