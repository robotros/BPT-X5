/*******************************************************************************
* Title: VU_Control.h                                                     
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the function prototypes that are used to control the animations                                                                             
* that are used with the VU meter attachment.                                                                                                                                                         
*******************************************************************************/
   
#ifndef VU_CONTROL_H
#define VU_CONTROL_H
 
/*************************************************
*                   Constants                    *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/
void Pods_VU_Mode1(UINT16 *signal);
void Pods_VU_Mode2(UINT16 signal);

void Grid_VU_Mode1(UINT16 signal);
void Grid_VU_Mode2(UINT16 *signal);

void Rings_VU_Mode1(UINT16 *signal);

void Display_Data(UINT16 *channel);
void Bargraph_Update(UINT16 signal);

#endif