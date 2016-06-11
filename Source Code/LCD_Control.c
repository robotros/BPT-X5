/*******************************************************************************
* Title: LCD_Control.c                                                                       
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                                        
*                                                                               
* Description:                                                                  
* This file contains all of the functions and routines that handle the 
* operation of the 16x2 LCD display in conjunction with the IR remote.                                                                               
*******************************************************************************/
#ifndef LCD_CONTROL_C
#define LCD_CONTROL_C

#include "Main_Includes.h"
#include "LCD_Setup.h"
#include "LCD_Control.h"
#include "LED_Control.h"
#include "IR_Controls.h"
#include "IR_Sensors.h"
#include "LED_Graphics.h"
#include "Grid_Setup.h"
#include "Delay_Setup.h"
#include "File_Handling.h"
#include <stdio.h>
#include <stdlib.h>

extern UINT8 pod_brightness;
extern volatile INT8 LCD_pmenu;
extern volatile INT8 LCD_cmenu;
extern volatile UINT8 VU_Meter;
extern volatile UINT8 ball_washers;
extern volatile UINT8 DA_grid[4];
extern volatile UINT8 DA_pods[4];
extern volatile UINT8 DA_rings[4];

extern volatile T16_FLAG FLAG1;
extern volatile UINT16 ring_brightness;

extern volatile UINT8 keypress;
extern volatile UINT32 IR_sensors;

extern volatile RGB COLOR[11]; 

/*******************************************************************************
* Function: LCD_Screensaver(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Screensaver(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"** Chexal.com **");
  LCD_Text(0,1,"Beer Pong Tables");
}

/*******************************************************************************
* Function: LCD_Settings(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Settings(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Settings");
} 

/*******************************************************************************
* Function: LCD_Ball_Washers(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Ball_Washers(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"Ball Washers");
  
  if (ball_washers == 1)
  	LCD_Text(0,1,"Enabled");
  
  else
  	LCD_Text(0,1,"Disabled");
} 

/*******************************************************************************
* Function: LCD_Firmware_Update(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Firmware_Update(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Update Firmware?");
} 

/*******************************************************************************
* Function: LCD_Brightness(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Brightness(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Table Brightness");
} 

/*******************************************************************************
* Function: LCD_Brightness_Pods(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Brightness_Pods(void)
{
	UINT8 percent;
	char str[4];
	
	percent = ((float)pod_brightness / POD_BRIGHTNESS_MAX) * 100;
	
	sprintf(str, "%u", percent);
	
	LCD_CLEAR();
  LCD_Text(0,0,"RGB Pods");
  LCD_Text(0,1,"Brightness: ");
  LCD_Text(11,1,str);
  LCD_Char('\%');
} 

/*******************************************************************************
* Function: LCD_Brightness_Rings(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Brightness_Rings(void)
{
	UINT8 percent;
	char str[4];
	
	percent = ((float)ring_brightness / RING_MAX) * 100;
	
	if (percent == 0)
		percent++;
	
	sprintf(str, "%u", percent);
	
	LCD_CLEAR();
  LCD_Text(0,0,"LED Rings");
  LCD_Text(0,1,"Brightness: ");
  LCD_Text(11,1,str);
  LCD_Char('\%');;
} 

/*******************************************************************************
* Function: LCD_Calibrate_Sensors(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Calibrate_Sensors(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Calibrate Sensors");
}  

/*******************************************************************************
* Function: LCD_Underlighting(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Underlighting(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Underlighting");
}

/*******************************************************************************
* Function: LCD_Pod_Animations(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Pod_Animations(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Pod Animations");
} 

/*******************************************************************************
* Function: LCD_Grid_Animations(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Grid_Animations(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Grid Animations");
} 

/*******************************************************************************
* Function: LCD_Ring_Animations(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Ring_Animations(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Ring Animations");
} 

/*******************************************************************************
* Function: LCD_VU_Meter_Menu(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Meter_Menu(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"VU Meter Menu");
}	

/*******************************************************************************
* Function: LCD_VU_Mode1(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Mode1(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"VU Mode #1");
} 

/*******************************************************************************
* Function: LCD_VU_Mode2(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Mode2(void)
{
	LCD_CLEAR();
  LCD_Text(0,0,"VU Mode #2");
} 

/*******************************************************************************
* Function: LCD_VU_Mode3(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Mode3(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"VU Mode #3");
} 

/*******************************************************************************
* Function: LCD_VU_Mode4(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Mode4(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"Custom Animation");
} 

/*******************************************************************************
* Function: LCD_VU_Random(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_VU_Random(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"VU Mode Random");
}  

/*******************************************************************************
* Function: LCD_Scoreboard_Menu(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Scoreboard_Menu(void)
{	
	LCD_CLEAR();
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"Show Scoreboard");
} 

/*******************************************************************************
* Function: LCD_Show_Scoreboard(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_Show_Scoreboard(void)
{
	UINT8 i;
	UINT8 master = 0;
  UINT8 secondary = 0;
  char temp_str[2];
  
  //Add up the score for each side
	for (i = 0;i < 10;i++)
	{
	  master += ((IR_sensors >> i) & 0x01);
	  secondary += ((IR_sensors >> (i+10)) & 0x01);
	} 
	
	LCD_CLEAR();	
  LCD_Text(0,0,"*HOME **** AWAY*");
  
  if (master == 10)
  	LCD_Text(2,1,"10");
  
  else
  {
	  temp_str[0] = 0x30 + master;
	  temp_str[1] = '\0';
	  
  	LCD_Text(2,1,temp_str);
  }	
  
  if (secondary == 10)
  	LCD_Text(12,1,"10");
  
  else
  {
	  temp_str[0] = 0x30 + secondary;
	  temp_str[1] = '\0';
	  
  	LCD_Text(13,1,temp_str);
  }
  
  Delay_ms(1000);
} 
/*******************************************************************************
* Function: LCD_File_Transfer(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_File_Transfer(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"Transfer File");
} 

/*******************************************************************************
* Function: LCD_SD_Animation(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_SD_Animation(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"Run Animation");
} 

/*******************************************************************************
* Function: LCD_SD_Card_Menu(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a basic screen on the LCD display.                                                                            
*******************************************************************************/
void LCD_SD_Card_Menu(void)
{
	LCD_CLEAR();	
  LCD_Text(0,0,"**** Chexal ****");
  LCD_Text(0,1,"SD Card");
} 

/*******************************************************************************
* Function: LCD_Bootloader_Msg(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display the bootloader message before the PIC resets itself.                                                                            
*******************************************************************************/
void LCD_Bootloader_Msg(void)
{
	//Disable all features on the table
	Disable_All_Features();
	
	//Display info to enter the bootloader															
	LCD_CLEAR();
	LCD_Text(0,0,"Hold ");
	LCD_Char(0x7E);
	LCD_Text(7,0,"key");
	LCD_Text(0,1,"on restart");
	Delay_ms(1500);
	
	//Display second screen of info
	LCD_CLEAR();
	LCD_Text(0,0,"for 2 seconds to");
	LCD_Text(0,1,"enter bootloader");
	Delay_ms(1500);
	
	//Prepare for reboot
	LCD_CLEAR();
	LCD_Text(0,0,"Rebooting in...");
	LCD_Text(6,1,"Hold ");
	LCD_Char(0x7E);
	LCD_Text(13,1,"key");	
	
	LCD_Text(0,1,"3");
	Delay_ms(1000);
	
	LCD_Text(0,1,"2");	
	Delay_ms(1000);
	
	LCD_Text(0,1,"1");
	Delay_ms(700);
	
	//Alert the user to start transferring the hex data
	LCD_CLEAR();
	LCD_Text(0,0,"Transfer F/W");
	LCD_Text(0,1,"from PC...");
} 

/*******************************************************************************
* Function: Update_LCD_Menu(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will update the LCD menu according to what key was pressed on the
* IR controller. The LCD menu location is tracked with the use of the 'pmenu' and
* 'cmenu' variables.                                                                            
*******************************************************************************/
void Update_LCD_Menu(void)
{
  //Check which menu the LCD display should be displaying and adjust it accordingly
	switch (LCD_pmenu)
	{
		//Screensaver Menu
		case MENU_SCREENSAVER: 
										LCD_Screensaver(); 
										break;
		
		//Settings Menu
		case MENU_SETTINGS: 
										 //Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: LCD_Ball_Washers(); 					break;
											case 1: LCD_Firmware_Update(); 		break;
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Settings();			break;
										}	
										
										break;
		
		//RGB Pod and LED Rings Brightness Menu
		case MENU_BRIGHTNESS: 
										//Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: LCD_Brightness_Pods(); 		break;
											case 1: LCD_Brightness_Rings(); 	break;
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Brightness();			break;
										}	
										
										break;
		
		//VU Meter Modes Menu
		case MENU_VU_METER: 
										//Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: LCD_VU_Mode1(); 		break;
											case 1: LCD_VU_Mode2(); 		break;
											case 2: LCD_VU_Mode3(); 		break;
											case 3: LCD_VU_Mode4(); 		break;
											case 4: LCD_VU_Random(); 		break;
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_VU_Meter_Menu();			break;
										}	
										
										break;
		
		//RGB Underlighting Colors Menu
		case MENU_UNDERLIGHTING: 
										//Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Red"); 		break;
															
											case 1: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Green"); 		break;
															
											case 2: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Blue"); 		break; 
															
											case 3: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Cyan"); 		break; 
															
											case 4: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Magenta"); 		break; 
															
											case 5: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Yellow"); 		break; 
															
											case 6: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Pink"); 		break; 
															
											case 7: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Orange"); 		break; 
															
											case 8: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Violet"); 		break; 
															
											case 9: 
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"White"); 		break; 
															
											case 10:
															LCD_CLEAR();
															LCD_Text(0,0,"Underlight Color");
															LCD_Text(0,1,"Black (Off)"); 		break; 
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Underlighting();			break;
										}	
										
										break;
		
		//RGB Pod Animation Selection Menu
		case MENU_POD_ANIMATIONS: 
										 //Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Pod Animations");
															LCD_Text(0,1,"Cycle Animations"); 		break;
															
											case 1: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Pod Animations");
															LCD_Text(0,1,"Cup Detection"); 		break;
															
											case 2: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Pod Animations");
															LCD_Text(0,1,"Color Throb"); 		break; 
															
											case 3: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Pod Animations");
															LCD_Text(0,1,"Cycle 4 Colors"); 		break;  
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Pod_Animations();			break;
										}	
										
										break;
		
		//RGB Underlighting Colors Menu
		case MENU_GRID_ANIMATIONS: 
										 //Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Grid Animations");
															LCD_Text(0,1,"Cycle Animations"); 		break;
															
											case 1: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Grid Animations");
															LCD_Text(0,1,"Scoreboard"); 		break;
															
											case 2: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Grid Animations");
															LCD_Text(0,1,"Pong"); 		break; 
															
											case 3: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Grid Animations");
															LCD_Text(0,1,"Checkers"); 		break;   
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Grid_Animations();			break;
										}	
										
										break;
		
		//RGB Underlighting Colors Menu
		case MENU_RING_ANIMATIONS: 
										 //Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Ring Animations");
															LCD_Text(0,1,"Cycle Animations"); 		break;
															
											case 1: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Ring Animations");
															LCD_Text(0,1,"Ring Chase"); 		break;
															
											case 2: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Ring Animations");
															LCD_Text(0,1,"Crossfade"); 		break; 
															
											case 3: 			
															LCD_CLEAR();
															LCD_Text(0,0,"Ring Animations");
															LCD_Text(0,1,"Rotate Rings"); 		break;   
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_Ring_Animations();			break;
										}	
										
										break;
		
		//SD Card Menu
		case MENU_SD_CARD: 
										 //Display a child menu if one is active
										switch (LCD_cmenu)
										{
											case 0: LCD_SD_Animation(); 		break;
											case 1: LCD_File_Transfer(); 	break;
											
											//No child menus are active, display the parent menu
											case MENU_INACTIVE: LCD_SD_Card_Menu();			break;
										}	
										
										break;
										
		//IR Sensor Calibration Menu
		case MENU_CALIBRATE: 
										LCD_Calibrate_Sensors(); 
										break;
										
		//Scoreboard Menu
		case MENU_SCOREBOARD: 
										LCD_Scoreboard_Menu(); 
										break;

	 //This should never execute but it's here just in case there is a corrupt variable 
	 default: LCD_pmenu = 0; LCD_cmenu = 0; break;
	}					
		
}	 
					
/*******************************************************************************
* Function: Handle_Key_Command(UINT32 cmd)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will handle any key presses by executing the selected function 
* and updating the LCD menu handlers 'pmenu' and 'cmenu'.                                                                            
*******************************************************************************/
void Handle_Key_Command(UINT32 cmd)
{
	//Check to see if a shortcut key was pressed; If one was, change 'pmenu' and 'cmenu'
	//settings to go to that shortcut menu. 
	switch (cmd)
	{
		//OFF key was pressed, disable all features on table
		case KEY_OFF: 
								  MODE_STANDBY = ON;
                  Disable_All_Features();  	break;
		
		//ON key was pressed, turn on table
		case KEY_ON:  MODE_STANDBY = OFF;		break;
		
		//SD shortcut key was pressed, automatically navigate to SD menu
		case KEY_SD: 
												LCD_pmenu = MENU_SD_CARD; 
												LCD_cmenu = MENU_ACTIVE;
												cmd = KEY_UNRECOGNIZED; 			          	break;
		
		//VU shortcut key was pressed, automatically navigate to VU Meter menu
		case KEY_VU: 
												LCD_pmenu = MENU_VU_METER; 
												LCD_cmenu = MENU_VU_MODE1;
												cmd = KEY_UNRECOGNIZED; 			          	break;
		
		//MODE shortcut key was pressed, automatically navigate to Mode menu
		case KEY_MODE: 
												//LCD_pmenu = MENU_CALIBRATE; 
												//LCD_cmenu = MENU_INACTIVE;
												
												VU_Meter++;
												
												if (VU_Meter > 4)
													VU_Meter = 0;
												
												cmd = KEY_UNRECOGNIZED; 			          	break;
		
		//Settings shortcut key was pressed, automatically navigate to settings menu
		case KEY_SETTINGS: 
												LCD_pmenu = MENU_SETTINGS; 
												LCD_cmenu = MENU_BALLWASHERS;
												cmd = KEY_UNRECOGNIZED; 			          	break;
		
		//SCORE shortcut key was pressed, automatically navigate to menu and execute function
		case KEY_SCORE: 
												//LCD_pmenu = MENU_CALIBRATE; 
												//LCD_cmenu = MENU_INACTIVE;
												cmd = KEY_UNRECOGNIZED; 			          	break;
		
		//CAL shortcut key was pressed, automatically navigate to menu and execute function
		case KEY_CALIBRATE: 
												LCD_pmenu = MENU_CALIBRATE; 
												LCD_cmenu = MENU_INACTIVE;
												cmd = KEY_ENTER; 						break;															
	}											
	
	//Check to see what key was pressed and adjust the menu accordingly
	switch (LCD_pmenu)
	{
    /*************************************************
    *             LCD Main Screen Display
    *************************************************/
		case MENU_SCREENSAVER: 
						
						//If the up or down arrow key was pressed, go to the next parent menu
						if (cmd == KEY_UP) 
							LCD_pmenu++; 
							
					  else if (cmd == KEY_DOWN) 
							LCD_pmenu--; 
							
						//Rollover to the next valid menu if we have extended passed the menu range
						if (LCD_pmenu > PARENT_MENU_MAX)
							LCD_pmenu = PARENT_MENU_MIN;
						
						//Rollover to the next valid menu if we have extended passed the menu range
						if (LCD_pmenu < PARENT_MENU_MIN)
							LCD_pmenu = PARENT_MENU_MAX;	break;
		
		
    /*************************************************
    * Settings Parent/Child Menu Handler
    *************************************************/
		case MENU_SETTINGS: 
		
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = MENU_BALLWASHERS; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									//If any arrow was pressed, go to other menu (only 2 menus here)
									case KEY_UP:   
									case KEY_DOWN:  LCD_cmenu = (LCD_cmenu == MENU_BOOTLOADER) ? MENU_BALLWASHERS : MENU_BOOTLOADER; 
											 break;
									
									//Enter was pressed, perform selected function
									case KEY_ENTER: 
									
															//Toggle the activation of the ball washers
															if (LCD_cmenu == MENU_BALLWASHERS && BW_ACTIVE == 0)
																ball_washers = ~ball_washers;
													
																
															else if (LCD_cmenu == MENU_BOOTLOADER)
															{	
																//Display bootloader message
																//LCD_Bootloader_Msg();
																
																//Note: To enter the bootloader, we must hold down
																//any key on the infrared remote and point it at the
																//IR receiver on the table. An IR signal on startup 
																//is what tells the PIC to execute the bootloader
																//instead of the main application.
																//Reset the PIC with an illegal instruction, 
																//allowing us to enter the bootloader. The 'reset',
																// goto(0x0000) or goto(0x0200) will not enter the bootloader.
																asm("goto 0x0002");
																
															} 	break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					   		}
					 }  break;
		
   
    /*************************************************
    *  Pods/Rings Brightness Parent/Child Menu Handler
    *************************************************/
		case MENU_BRIGHTNESS: 
		
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = MENU_ACTIVE; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									//If any arrow was pressed, go to other menu (only 2 menus here)
									case KEY_UP:   
									case KEY_DOWN:  LCD_cmenu = (LCD_cmenu == 1) ? 0 : 1; break;
									
									//Enter was pressed, perform selected function
									case KEY_LEFT: 
															//Adjust the RGB pods brightness
															if (LCD_cmenu == 0)
															{
																pod_brightness--;
																Adjust_Pod_Brightness(&pod_brightness);
															}	
															
															//Adjust the LEDx channels brightness (LED rings)	
															else if (LCD_cmenu == 1)
															{
																if (ring_brightness > 5000)
																   ring_brightness -= 5000;	
																   
																else
																	ring_brightness = 400;
															}		
															
															break;
									
									//Enter was pressed, perform selected function
									case KEY_RIGHT: 
															//Adjust the RGB pods brightness
															if (LCD_cmenu == 0)
															{
																pod_brightness++;
																Adjust_Pod_Brightness(&pod_brightness);
															}	
															
															//Adjust the LEDx channels brightness (LED rings)	
															else if (LCD_cmenu == 1)
															{
																if (ring_brightness < 60500)
																   ring_brightness += 5000;	
																   
																else
																	ring_brightness = RING_MAX;
															}	   
															
															break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					   		}
					 }  break;	
   
   
    /*************************************************
    * IR Sensor Calibration Parent/Child Menu Handler
    *************************************************/
		case MENU_CALIBRATE: 
						
						//Check to see which key was pressed
						switch (cmd)
						{	
							case KEY_UP:  
													//If the up arrow was pressed, go to the next parent menu
													LCD_pmenu++; 
													
													//If we extended passed the menu range, rollover to next menu
													if (LCD_pmenu > PARENT_MENU_MAX)
														LCD_pmenu = PARENT_MENU_MIN;	break;
							
							case KEY_DOWN:  
													//If the down arrow was pressed, go to the next parent menu
													LCD_pmenu--; 
						
													//If we extended passed the menu range, rollover to next menu
													if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;

							//Enter was pressed, perform selected function
							case KEY_ENTER: 
													LCD_CLEAR();
													LCD_Text(0,0,"Calibrating...");
												
													//Calibrate the infrared sensors
													Sensor_Calibration();
													LCD_Text(0,1,"Complete");
													Delay_ms(400); 
													
													break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					   		} break;
   
   
    /*************************************************
    * Animation Handler Menu for the RGB Pods
    *************************************************/
		case MENU_POD_ANIMATIONS: 
		
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = MENU_ACTIVE; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									//If any arrow was pressed, go to other menu (only 2 menus here)
									case KEY_UP: 
																//Go the the next higher menu  
																LCD_cmenu++;
																
																//Check to see if we have passed the menu range
																if (LCD_cmenu > MENU_POD_RANGE_MAX)
																	LCD_cmenu = MENU_POD_RANGE_MIN;
																
																
																
									 							break;  								
									
									case KEY_DOWN:
																//Go the the next lower menu  
																LCD_cmenu--;
																
																//Check to see if we have passed the menu range
																if (LCD_cmenu < MENU_POD_RANGE_MIN)
																	LCD_cmenu = MENU_POD_RANGE_MAX;
																
									 							break;
									 							
									 							
									//Enter was pressed, perform selected function
									case KEY_ENTER: 
															
//															//TEST FUNCTIONS
//															switch (LCD_cmenu)
//															{
//																case 0:	DA_pods[0] = 0;		break;
//																				
//																case 1: 
//																				DA_Pods[0] = 1;
//																				DA_Pods[1] = 0;	break;
//																				
//																case 2: 
//																				DA_Pods[0] = 2;
//																				DA_Pods[1] = 0;	break;
//																				
//															}	break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;	 		
									
									//Enter was pressed, perform selected function
									case KEY_LEFT: 
									
															////TEST FUNCTIONS
															if (LCD_cmenu == 0)
															{
																pod_brightness--;
																Adjust_Pod_Brightness(&pod_brightness);
															}	
																
															else if (LCD_cmenu == 1)
															{
																if (ring_brightness > 5000)
																   ring_brightness -= 5000;	
																   
																else
																	ring_brightness = 400;
															}		
															
															break;
									
									//Enter was pressed, perform selected function
									case KEY_RIGHT: 
									
															////TEST FUNCTIONS
															if (LCD_cmenu == 0)
															{
																pod_brightness++;
																Adjust_Pod_Brightness(&pod_brightness);
															}	
		
															else if (LCD_cmenu == 1)
															{
																if (ring_brightness < 60500)
																   ring_brightness += 5000;	
																   
																else
																	ring_brightness = RING_MAX;
															}	   
															
															break;					 	            
					   		}
					 }  break;	
					   		    
	  /*************************************************
    * Scoreboard Parent/Child Menu Handler
    *************************************************/
		case MENU_SCOREBOARD: 
						
						//Check to see which key was pressed
						switch (cmd)
						{	
							case KEY_UP:  
													//If the up arrow was pressed, go to the next parent menu
													LCD_pmenu++; 
													
													//If we extended passed the menu range, rollover to next menu
													if (LCD_pmenu > PARENT_MENU_MAX)
														LCD_pmenu = PARENT_MENU_MIN;	break;
							
							case KEY_DOWN:  
													//If the down arrow was pressed, go to the next parent menu
													LCD_pmenu--; 
						
													//If we extended passed the menu range, rollover to next menu
													if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;

							//Enter was pressed, perform selected function
							case KEY_ENTER: 
													LCD_Show_Scoreboard();	break;
									
							//Cancel out of all menus and go back to the screensaver
							case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					  } break;  
   
   
    /*************************************************
    *        VU Meter Parent/Child Menu Handler
    *************************************************/
		case MENU_VU_METER: 
		
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = 0; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_cmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_cmenu > MENU_VU_RANGE_MAX)
																LCD_cmenu = MENU_VU_RANGE_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_cmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_cmenu < MENU_VU_RANGE_MIN)
																LCD_cmenu = MENU_VU_RANGE_MAX;   break;
									
									//Enter was pressed, perform selected function
									case KEY_ENTER: 
									
															//Navigate to the current child menu
															switch (LCD_cmenu)
															{
																case MENU_VU_MODE1: 				
																				LCD_CLEAR();
																				LCD_Text(0,0,"VU Mode #1 Set");
																				VU_Meter = 2;
																				Delay_ms(300);	break;
																				
																case MENU_VU_MODE2: 				
																				LCD_CLEAR();
																				LCD_Text(0,0,"VU Mode #2 Set");
																				VU_Meter = 3;
																				Delay_ms(300);	break;
																				
																case MENU_VU_MODE3: 			
																				LCD_CLEAR();
																				LCD_Text(0,0,"VU Mode #3 Set");
																				VU_Meter = 4;
																				Delay_ms(300);	break;
																
																//This is the LCD menu handler. Add your code in the main loop
																//to the 4th case statement				
																case MENU_VU_MODE4: 			
																				LCD_CLEAR();
																				LCD_Text(0,0,"Back To Default");
																				LCD_Text(0,1,"Activated");		
  																			VU_Meter = 0;																				
																				Delay_ms(300);	break;
																				
																case MENU_VU_RANDOM: 				
																				LCD_CLEAR();
																				LCD_Text(0,0,"VU Mode Random Set");	
  																			VU_Meter = 0;								
																				Delay_ms(1000);	break;
															}	break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; 
																	 VU_Meter = 0;
																	 break;							 	            
					   		}
					 }  break;    
					 
	  /*************************************************
    *   RGB Underlighting Parent/Child Menu Handler
    *************************************************/
		case MENU_UNDERLIGHTING: 
			
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = 0; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_cmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_cmenu > 10)
																LCD_cmenu = 0;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_cmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_cmenu < 0)
																LCD_cmenu = 10;   break;
									
									//Enter was pressed, perform selected function
									case KEY_ENTER: 
															
															//TEST FUNCTIONS
															switch (LCD_cmenu)
															{
																case 0: 	
																				RGB_Underlighting(COLOR[RED]);	break;
																				
																case 1: 			
																				RGB_Underlighting(COLOR[GREEN]);	break;
																				
																case 2: 				
																				RGB_Underlighting(COLOR[BLUE]);	break;
																				
																case 3: 	
																				RGB_Underlighting(COLOR[CYAN]);	break;
																				
																case 4: 	
																				RGB_Underlighting(COLOR[MAGENTA]);	break;
																				
																case 5: 
																				RGB_Underlighting(COLOR[YELLOW]);	break;
																				
																case 6: 
																				RGB_Underlighting(COLOR[PINK]);	break;
																				
																case 7: 
																				RGB_Underlighting(COLOR[ORANGE]);	break;
																				
																case 8: 
																				RGB_Underlighting(COLOR[VIOLET]);	break;
																				
																case 9: 	
																				RGB_Underlighting(COLOR[WHITE]);	break;
																				
																case 10: 
																				RGB_Underlighting(COLOR[BLACK]);	break;
																				
															}	break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					   		}
					 }  break;
   
   
    /************************************************
    *       SD Card Parent/Child Menu Handler
    ************************************************/
		case MENU_SD_CARD: 
		
						//If the child menus are inactive, display the parent menu
						if (LCD_cmenu == MENU_INACTIVE)
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									case KEY_UP:  
															//If the up arrow was pressed, go to the next parent menu
															LCD_pmenu++; 
															
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu > PARENT_MENU_MAX)
																LCD_pmenu = PARENT_MENU_MIN;	break;
									
									case KEY_DOWN:  
															//If the down arrow was pressed, go to the next parent menu
															LCD_pmenu--; 
						
															//If we extended passed the menu range, rollover to next menu
															if (LCD_pmenu < PARENT_MENU_MIN)
																LCD_pmenu = PARENT_MENU_MAX;   break;
									
									//Enter the parent menu, thus activating the child menus
									case KEY_ENTER:	LCD_cmenu = 0; 	break;
									
									//Cancel out of the parent menu and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER;  break;	            
					   		} 		
					  }  			
						
						//A child menu is open; Determine what selection was made	
						else
						{
								//Check to see which key was pressed
								switch (cmd)
								{
									//If any arrow was pressed, go to other menu (only 2 menus here)
									case KEY_UP:   
									case KEY_DOWN:  LCD_cmenu = (LCD_cmenu == 1) ? 0 : 1; break;
									
									//Enter was pressed, perform selected function
									case KEY_ENTER: 
									
															////TEST FUNCTIONS
															if (LCD_cmenu == 0)
															{
																LCD_CLEAR();
																LCD_Text(0,0,"Animation.chx");
																Delay_ms(1000);
															}	
													
																
															else if (LCD_cmenu == 1)
															{
																LCD_CLEAR();
																LCD_Text(0,0,"File Transferred");
																Delay_ms(1000);
															} 	break;
									
									//Cancel out of all menus and go back to the screensaver
									case KEY_CANCEL:
																	 LCD_cmenu = MENU_INACTIVE;
																	 LCD_pmenu = MENU_SCREENSAVER; break;							 	            
					   		}
					 }  break;
	}		                     
	
	//Reset the keypress its default state	
	keypress = KEY_UNRECOGNIZED;
	
	//Update the LCD menu
	Update_LCD_Menu();					
}	


#endif
