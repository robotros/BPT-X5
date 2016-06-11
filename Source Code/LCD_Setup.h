/*******************************************************************************
* Title: LCD_Setup.h                                                       
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the constants and function prototypes which are used to 
* configure the operation of a standard (HD44780) 16x2 LCD Display.                                                                  
*******************************************************************************/
   
#ifndef LCD_SETUP_H
#define LCD_SETUP_H
 
/*************************************************
*                   Constants                    *
*************************************************/
//Pin Definitions
#define LCD_DC        _LATC9
#define LCD_RW        _LATB6
#define LCD_EN        _LATB5

#define LCD_D7        _LATG9
#define LCD_D6        _LATG8
#define LCD_D5        _LATG7
#define LCD_D4        _LATG6

//Clear the entire display and reset DDRAM address to 0
#define LCD_CLEAR_DISPLAY     0x01

//Return to original position, DDRAM remains unchanged
#define LCD_RETURN_HOME       0x02

//Set cursor move direction and specify display shift
#define LCD_ENTRY_MODE_SET    0x04
//The following bits must be OR'd with the above command to activate them
#define LCD_INCREMENT         0x02
#define LCD_DECREMENT         0x00
#define LCD_ACC_DISPLAY_SHIFT 0x01

//Sets diplay on/off, cursor on/off and blinking of cursor position
#define LCD_DISPLAY_CONTROL   0x08
//The following bits must be OR'd with the above command to activate them
#define LCD_DISPLAY_ON        0x04
#define LCD_DISPLAY_OFF       0x00
#define LCD_CURSOR_ON         0x02
#define LCD_CURSOR_OFF        0x00
#define LCD_BLINK_ON          0x01
#define LCD_BLINK_OFF         0x00

//Moves cursor and shifts display without changing DDRAM contents
#define LCD_CURSOR_DISPLAY    0x10
//The following bits must be OR'd with the above command to activate them
#define LCD_DISPLAY_SHIFT     0x08
#define LCD_CURSOR_MOVE       0x00
#define LCD_SHIFT_RIGHT       0x04
#define LCD_SHIFT_LEFT        0x00

//Sets interface data length, number of display lines and font 
#define LCD_FUNCTION_SET      0x20
//The following bits must be OR'd with the above command to activate them
#define LCD_DATA_8BITS        0x10
#define LCD_DATA_4BITS        0x00
#define LCD_TWO_LINES         0x08
#define LCD_ONE_LINE          0x00
#define LCD_FONT_5x10         0x04
#define LCD_FONT_5x8          0x00

//Set CGRAM address. Data is sent after receiving this setting.
#define LCD_SET_CGRAM_ADDR    0x40

//Set DDRAM address. Data is sent after receiving this setting.
#define LCD_SET_DDRAM_ADDR    0x80

//Sent 3 times on startup to ensure proper 4-bit operation
#define LCD_STARTUP_4BIT			0x03
#define LCD_4BIT_FS						0x02
#define LCD_NO_DATA						0x00

//LCD Display Clear
#define LCD_CLEAR()				{LCD_CMD(LCD_CLEAR_DISPLAY); Delay_us(1500);}

/*************************************************
*                   Macros                       *
*************************************************/
#define LCD_PULSE(x)  {x = 1; Delay_us(80); x = 0;}

/*************************************************
*              Function Prototypes               *
*************************************************/
void LCD_Init(void);
void LCD_CMD(UINT8 cmd);
void LCD_Char(char symbol);
void LCD_Cursor_Pos(UINT8 x, UINT8 y);
void LCD_Text(UINT8 x, UINT8 y, char text[32]);
void LCD_Create_Character(UINT8 symbol[8], UINT8 loc);


#endif