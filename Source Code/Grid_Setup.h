/*******************************************************************************
* Title: Grid_Setup.h                                                           
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                          
*                                                                               
* Description:                                                                  
* This file conatins the function prototypes and definitions which are used to 
* control the operation of the LED grid. The maximum grid size that the X5 PCB  
* can control is 32x16. I only use a 32x12 LED grid on my tables.               
*******************************************************************************/

#ifndef GRID_SETUP_H
#define GRID_SETUP_H

/*************************************************
*               Bit Definitions                  *
*************************************************/
#define GRID_UPDATE           FLAG1.b4

/*************************************************
*                   Constants                    *
*************************************************/
#define GRID_X_MAX            32
#define GRID_Y_MAX            12

/*************************************************
*                   Macros                       *
*************************************************/
#define UPDATE_FRAME()      (GRID_UPDATE = 1)

/*************************************************
*              Function Prototypes               *
*************************************************/
void Grid_Init(void);
void Grid_Control(void);
void Shift_Grid_Left(UINT8 amount);
void Shift_Grid_Right(UINT8 amount);
void Grid_Frame_Update(UINT32 *data);

#endif
