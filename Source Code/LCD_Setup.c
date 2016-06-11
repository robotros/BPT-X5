/*******************************************************************************
* Title: LCD_Setup.h                                                       
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions and routines that are used to configure the
* operation of a standard (HD44780) 16x2 LCD Display and control it.                                                
*******************************************************************************/

#ifndef LCD_SETUP_C
#define LCD_SETUP_C

#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "LCD_Setup.h"

/*******************************************************************************
* Function: LCD_Init(void)                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                   
* This function is set to initialize the LCD display with a 4-bit data bus,  
* cursor off and 16x2 screen.
*******************************************************************************/
void LCD_Init(void)
{
	//Allow LCD to stabilize
  Delay_ms(20);
  
  //Send specified startup sequence
  LCD_CMD(LCD_STARTUP_4BIT);
  Delay_ms(5);
  
  //Send specified startup sequence
  LCD_CMD(LCD_STARTUP_4BIT);
  Delay_ms(5);
  
  //Send specified startup sequence
  LCD_CMD(LCD_STARTUP_4BIT);
  Delay_ms(5);
  
  //Return home
  LCD_CMD(LCD_RETURN_HOME);
  Delay_ms(5);
  
  //Send Return Home again
  LCD_CMD(LCD_RETURN_HOME);
  Delay_ms(5);
  
  //Turn off all Display Control features for startup
  LCD_CMD(LCD_DISPLAY_CONTROL);
  Delay_ms(5);
  
  //Clear LCD Display
  LCD_CMD(LCD_RETURN_HOME);
  Delay_ms(5);
  
  //Set Entry Mode and cursor shifting
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_INCREMENT);
  Delay_ms(5);
  
  //Finish initializing with your settings
  LCD_CMD(LCD_FUNCTION_SET | LCD_DATA_4BITS | LCD_TWO_LINES);
  Delay_ms(5);
  
  //This was already set above. If you want to make a change, do it here.
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_INCREMENT);
  Delay_ms(5);
  
  //Display On, Cursor Off, Blinking Off
  LCD_CMD(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
  Delay_ms(5);
}

/*******************************************************************************
* Function: LCD_CMD(UINT8 cmd)                                                                     
*                                                                               
* Variables:                                                                    
* cmd -> An 8-bit command that is sent to the LCD display                                                                           
*                                                                               
* Description:                                                                  
* This function will write an 8-bit command to the LCD display in two operations,
* splitting the byte up into two nibbles.                                                                              
*******************************************************************************/
void LCD_CMD(UINT8 cmd)
{
	//This ensures that any current LCD operations have time to complete
  Delay_us(120);
  
  //Prep the data lines with the high nibble of the command
  _LATG9 = (cmd & 0x80) >> 7;
  _LATG8 = (cmd & 0x40) >> 6;
  _LATG7 = (cmd & 0x20) >> 5;
  _LATG6 = (cmd & 0x10) >> 4;
  
  //Set the LCD into command mode
  LCD_DC = 0;
  LCD_RW = 0;
  
  //Clock the data into the LCD
  Delay_us(120);
  LCD_PULSE(LCD_EN);
  Delay_us(120);
  
  //Prep the data lines with the low nibble of the command
  _LATG9 = (cmd & 0x08) >> 3;
  _LATG8 = (cmd & 0x04) >> 2;
  _LATG7 = (cmd & 0x02) >> 1;
  _LATG6 = (cmd & 0x01);
  
  //Clock the data into the LCD
  Delay_us(120);
  LCD_PULSE(LCD_EN);
}

/*******************************************************************************
* Function: LCD_Send(UINT8 data)                                                                     
*                                                                               
* Variables:                                                                    
* data -> An 8-bit piece of data that is to be sent to the LCD display                                                                           
*                                                                               
* Description:                                                                  
* This function is used to send data to the LCD display. This allows the user to
* display text/symbols on the LCD.                                                                              
*******************************************************************************/
void LCD_Send(UINT8 data)
{
	//This ensures that any current LCD operations have time to complete
  Delay_us(120);
  
  //Prep the data lines with the high nibble of the data
  _LATG9 = (data & 0x80) >> 7;
  _LATG8 = (data & 0x40) >> 6;
  _LATG7 = (data & 0x20) >> 5;
  _LATG6 = (data & 0x10) >> 4;
  
  //Set the LCD into data mode
  LCD_DC = 1;
  LCD_RW = 0;
  Delay_us(120);
  
  //Clock the data into the LCD
  LCD_PULSE(LCD_EN);
  Delay_us(120);
  
  //Prep the data lines with the low nibble of the data
  _LATG9 = (data & 0x08) >> 3;
  _LATG8 = (data & 0x04) >> 2;
  _LATG7 = (data & 0x02) >> 1;
  _LATG6 = (data & 0x01);
  
  //Clock the data into the LCD
  Delay_us(120);
  LCD_PULSE(LCD_EN);
}

/*******************************************************************************
* Function: LCD_Cursor_Pos(UINT8 x, UINT8 y)                                                                     
*                                                                               
* Variables:                                                                    
* x -> The x-position of the cursor on the LCD                                  
* y -> The y-position of the cursor on the LCD                                                                           
*                                                                               
* Description:                                                                  
* This function can be used to set the LCD cursors position at the location (x,y).                                                                              
*******************************************************************************/
void LCD_Cursor_Pos(UINT8 x, UINT8 y)
{
  //Calculate the DDRAM location (0x40 is line 2 in DDRAM)
  x += (y * 0x40);
  
  //Set the cursor position at the (x,y) coordinate
  LCD_CMD(LCD_SET_DDRAM_ADDR | x);
}

/*******************************************************************************
* Function: LCD_Text(UINT8 x, UINT8 y, char text[32])                                                                     
*                                                                               
* Variables:                                     
* x -> The x-position of the cursor on the LCD                                  
* y -> The y-position of the cursor on the LCD                                                                 
* text -> The text that is to be displayed on the LCD                                                                         
*                                                                               
* Description:                                                                  
* This function is used to display text at an (x,y) coordinate on the LCD.                                                                               
*******************************************************************************/
void LCD_Text(UINT8 x, UINT8 y, char text[32])
{
  UINT8 i = 0;
  
  //Set the LCD cursor position
  LCD_Cursor_Pos(x,y);
  
  //Write the full string to the LCD display
  while (text[i] != '\0')
  {
    LCD_Send(text[i]);
    i++;
  }  
}

/*******************************************************************************
* Function: LCD_Char(char symbol)                                                                     
*                                                                               
* Variables:                                                                    
* symbol -> the character that is to be displayed on the LCD                                                                           
*                                                                               
* Description:                                                                  
* This function will display one character on the LCD display.                                                                              
*******************************************************************************/
void LCD_Char(char symbol)
{
  //Send the character to the LCD display
  LCD_Send(symbol);  
}

/*******************************************************************************
* Function: LCD_Create_Character(UINT8 symbol[8], UINT8 loc)                                                                     
*                                                                               
* Variables:                                                                    
* symbol -> The 5x7 character/symbol that is to be generated                                                                          
* loc -> The location where the symbol is stored
*                                                                              
* Description:                                                                  
* This function will create a symbol and save it in the LCD RAM. The LCD allows
* one to create 8 symbols and store them in RAM at the same time.                                                                              
*******************************************************************************/
void LCD_Create_Character(UINT8 symbol[8], UINT8 loc)
{
  UINT8 i;
  
  //Navigate to the address where the character will be stored
  LCD_CMD(LCD_SET_CGRAM_ADDR + (loc * 8));
  Delay_us(60);
  
  //Write the character into the RAM
  for (i = 0;i < 8;i++)
    LCD_Send(symbol[i]);
  
  //Allow the operation to finish  
  Delay_us(37);
}

#endif