/*******************************************************************************
* Title: LCD_Control.h                                                                       
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                                        
*                                                                               
* Description:                                                                  
* This file contains all of the constants and definitions that handle the 
* operation of the 16x2 LCD display in conjunction with the IR remote.                                                                               
*******************************************************************************/

#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

/*************************************************
*                   Constants                    *
*************************************************/
#define MENU_INACTIVE								100
#define MENU_ACTIVE									0
					
#define PARENT_MENU_MIN							0
#define PARENT_MENU_MAX	  					6

#define MENU_SCREENSAVER						0
#define MENU_BRIGHTNESS							1	
#define MENU_CALIBRATE							2	
#define MENU_SCOREBOARD							91 //Not Implemented Yet
#define MENU_POD_ANIMATIONS					92 //Not Implemented Yet
#define MENU_GRID_ANIMATIONS				93 //Not Implemented Yet
#define MENU_RING_ANIMATIONS				94 //Not Implemented Yet
#define MENU_VU_METER								3
#define MENU_SD_CARD								6 //Not Implemented Yet
#define MENU_UNDERLIGHTING					4	
#define MENU_SETTINGS								5	

#define MENU_POD_RANGE_MIN					0
#define MENU_POD_RANGE_MAX					3

#define MENU_GRID_RANGE_MIN					0
#define MENU_GRID_RANGE_MAX					3

#define MENU_RING_RANGE_MIN					0
#define MENU_RING_RANGE_MAX					3

				
#define	MENU_BALLWASHERS						0
#define MENU_BOOTLOADER							1
#define MENU_SENSITIVITY						2
				
#define MENU_VU_MODE1								0
#define MENU_VU_MODE2								1
#define MENU_VU_MODE3								2
#define MENU_VU_MODE4								3
#define MENU_VU_RANDOM							4
#define MENU_VU_RANGE_MIN						0
#define MENU_VU_RANGE_MAX						3

/*************************************************
*                   Macros                       *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/
void LCD_VU_Meter_Menu(void);
void LCD_VU_Mode1(void);
void LCD_VU_Mode2(void);
void LCD_VU_Mode3(void);
void LCD_VU_Mode4(void);
void LCD_VU_Random(void);
void LCD_SD_Card_Menu(void);
void LCD_File_Transfer(void);
void LCD_SD_Animation(void);
void LCD_Bootloader_Msg(void);
void LCD_Underlighting(void);


void LCD_Pod_Animations(void);
void LCD_Grid_Animations(void);
void LCD_Ring_Animations(void);

void LCD_Screensaver(void);
void LCD_Brightness(void);
void LCD_Ball_Washers(void);
void LCD_Firmware_Update(void);
void LCD_Settings(void);
void LCD_Brightness_Rings(void);
void LCD_Brightness_Pods(void);
void LCD_Show_Scoreboard(void);
void LCD_Scoreboard_Menu(void);

void Handle_Key_Command(UINT32 cmd);

#endif
