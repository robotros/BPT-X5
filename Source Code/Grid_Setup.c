/*******************************************************************************
* Title: Grid_Setup.c                                                            
* Version: 1.0                                                                   
* Author: Jeff Nybo                                                              
* Date: March 13, 2015                                                           
*                                                                                
* Description:                                                                   
* This file contains the functions that are used to control the LED grid and the
* 74HC595 shift registers that drive the LEDs.                                  
*******************************************************************************/

#ifndef GRID_SETUP_C
#define GRID_SETUP_C

#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "74HC595_Setup.h"
#include "Grid_Setup.h"

/*************************************************
*               Global Variables                 *
*************************************************/
extern volatile UINT8 frame_update;

extern volatile T16_FLAG FLAG1;

extern volatile UINT32 grid_row[12];
extern volatile UINT32 grid_frame[12];

/*******************************************************************************
* Function: Grid_Init(void)                                                                   * 
*                                                                            
* Variables:                                                                 
* N/A                                                                        
*                                                                            
* Description:                                                               
* Prepare the 74HC595's to receive data.                                                                         
*******************************************************************************/
void Grid_Init(void)
{ 
	//Enable each 74HC595
  HC595_EN = 0;
}

/*******************************************************************************
* Function: Grid_Frame_Update(UINT32 *data)                                                                 
*                                                                           
* Variables:                                                                
* *data -> Contains the pixel data for one frame on the LED grid                                                                      
*                                                                           
* Description:                                                              
* This function will update the frame data for the LED grid. The frame data will
* consist of a bit for each pixel. For example:
*
* By default, the LED grid size is 32x12 which is 384 pixels. Each pixel is
* either ON or OFF, meaning it can be represented by 1 bit. 8-bits make up
* one byte, so 384pixels / 8bitsperpixel = 48 bytes. 
*
* So one frame of data for a 32x12 LED grid is represented with 48 bytes.                                                                         
*******************************************************************************/
void Grid_Frame_Update(UINT32 *data)
{ 
  UINT8 i;
  
  //Copy the passed in data into the grids frame data
  for (i = 0;i < GRID_Y_MAX;i++)
    grid_frame[i] = data[i];
}

/*******************************************************************************
* Function: Grid_Control(void)                                                                   
*                                                                             
* Variables:                                                                  
* N/A                                                                         
*                                                                             
* Description:                                                                
* This function controls the refreshing of the LED grid. Seeing as the LED grid
* is being multiplexed, this function will cycle through each LED grid row
* continuously and update the frame data in the process. The refresh rate of
* the LED grid is determined by Timer5.                                                                             
*******************************************************************************/
void Grid_Control(void)
{   
  UINT8 i;
  UINT8 buf[6];
  static UINT8 row = 0;
  
  //Separate the frame data into byte sized sections for the 74HC595's
  buf[2] = (UINT8)(grid_frame[row] >> 24);
  buf[3] = (UINT8)(grid_frame[row] >> 16);
  buf[4] = (UINT8)(grid_frame[row] >> 8);
  buf[5] = (UINT8)grid_frame[row];
  
  //If the row being updated is any row between row[0] and row[7], activate
  //the specified column on IC10 which will activate that current row
  if (row < 8)
  {
	  //Disable all of the rows on IC13 by putting all outputs low
    buf[0] = 0x00;
    
    //Only set one output high on IC10, which will activate that specific row
    buf[1] = 1 << row;
  }
   
  //If the row being updated is above row[7], activate the specified column 
  //on IC13 which will activate that current row
  else
  {
    //Only set one output high on IC13, which will activate that specific row
    buf[0] = 1 << (row - 8);
    
	  //Disable all of the rows on IC10 by putting all outputs low
    buf[1] = 0x00;
  }  
  
  //Send all of the packed data to the 74HC595's to update the grid
  HC595_Send_Data(buf);
  
  //Update the next row on the next loop
  row++;
  
  //If all of the rows have been updated, a full refresh cycle (frame) has completed
  if (row >= GRID_Y_MAX)
  {
	  //Prepare to begin the cycle again
    row = 0;   
    
    //If there is new data to be sent to the grid, do it at the start of the frame
    //to prevent tearing on the screen
    if (GRID_UPDATE)
    {
	    //Update the frame data
      for (i = 0;i < GRID_Y_MAX;i++)
        grid_frame[i] = grid_row[i];
      
      //Reset the flag  
      GRID_UPDATE = 0;
    }    
  }
}

/*******************************************************************************
* Function: Shift_Grid_Right(UINT8 amount)                                                                   
*                                                                             
* Variables:                                                                  
* amount -> The amount of columns that the grid will be shifted over                                                                        
*                                                                             
* Description:                                                                
* This function will shift the grid 'amount' columns to the right. This function
* will only modify the grid data, to actually write it to the grid the user
* must call UPDATE_FRAME() after this function.                                                                            
*******************************************************************************/
void Shift_Grid_Right(UINT8 amount)
{  
  UINT8 i;
  
  //Shift the data on the grid x amount of times to the right
  for (i = 0;i < GRID_Y_MAX;i++)
    grid_row[i] <<= amount;              
}

/*******************************************************************************
* Function: Shift_Grid_Left(UINT8 amount)                                                                   
*                                                                             
* Variables:                                                                  
* amount -> The amount of columns that the grid will be shifted over                                                                        
*                                                                             
* Description:                                                                
* This function will shift the grid 'amount' columns to the left. This function
* will only modify the grid data, to actually write it to the grid the user
* must call UPDATE_FRAME() after this function.                                                                            
*******************************************************************************/
void Shift_Grid_Left(UINT8 amount)
{  
  UINT8 i;
  
  //Shift the data on the grid x amount of times to the left
  for (i = 0;i < GRID_Y_MAX;i++)
    grid_row[i] >>= amount;              
}
  
#endif
