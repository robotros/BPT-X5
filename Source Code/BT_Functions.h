/*******************************************************************************
* Title: BT_Functions.h                                                                      
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file contains various function prototypes and definitions which are used
* to control the operation of the UART.                                                                          
*******************************************************************************/

#ifndef BT_FUNCTIONS_H
#define BT_FUNCTIONS_H

/*************************************************
*               Bit Definitions                  *
*************************************************/
#define BT_STATE 									PORTCbits.RC7
#define BT_KEY										_LATC8
#define BT_TX											_LATC6
#define BT_RX											PORTBbits.RB9

#define UPDATE_UART       				FLAG1.b2

/*************************************************
*                   Constants                    *
*************************************************/
//Generic return codes				
#define BT_SUCCESS								0x00
#define BT_GENERAL_ERROR					0xFF

/*
//These values coincide with the commands contained in 'UART_CMD[25][32]' 
//in Globals.h and their locations in the array
#define BT_ACKNOWLEDGE            0                
#define BT_DEBUG_MENU             1               
#define BT_VERSION                2            
#define BT_POD                    3         
#define BT_RING                   4         
#define BT_PIXEL                  5            
#define BT_STANDBY                6            
#define BT_ACTIVE                 7            
#define BT_HELP_MENU	            8                
#define BT_POD_HELP_MENU	        9                      
#define BT_RING_HELP_MENU         10                  
#define BT_PIXEL_HELP_MENU        11                   
#define BT_EEPROM_HELP_MENU       12                    
#define BT_EEPROM_WRITE           13                
#define BT_EEPROM_READ            14               
#define BT_RESET					        15             
#define BT_SD_WRITE            		16                
#define BT_SD_READ            		17   
*/

//Bluetooth Control Commands
#define BT_GRID_CONTROL									0x0010  
#define BT_POD_CONTROL									0x0011
#define BT_LEDX_CONTROL									0x0012
			
#define BT_TEST_IR_VALUES								0x0020
#define BT_PRINT_KEYPRESS								0x0021
#define BT_RUN_PIXEL_CHECK							0X0022
#define BT_EEPROM_DUMP									0X0023
#define BT_FACTORY_RESET								0x0024
#define BT_ENUMERATE_SD									0x0025
#define BT_SD_CARD_SPECS								0x0026
			
#define BT_ACTIVE												0x002E
#define BT_STANDBY											0x002F

#define BT_INVALID_COMMAND							0xFE

//Receive buffer sizes; The amount of bytes to be passed after the initial command
#define BT_GRID_CONTROL_RX_BUF  				48     
#define BT_TEST_IR_VALUES_RX_BUF	 			0    
#define BT_ACTIVE_RX_BUF					 			0    
#define BT_STANDBY_RX_BUF					 			0      

/*************************************************
*              Function Prototypes               *
*************************************************/
void Help_Menu(void);
void Debug_Menu(void);
void Pod_Help_Menu(void);
void Pixel_Help_Menu(void);
void EEPROM_Help_Menu(void);
void BT_IR_Sensor_Data(void);
void Clear_UART_String(void);
void LED_Ring_Help_Menu(void);

void BT_Update_LED_Grid(UINT8 *data);

UINT8 Find_Command(char str[32]);
UINT8 Check_UART_Command(UINT8 *data);
UINT8 Receive_Buffer_Size(UINT8 *data);
UINT8 Find_Char(char str[32], char token);
void HC06_Init(char name[20], UINT32 baudrate);
UINT8 UART_Execute_Command(UINT8 cmd, UINT32 part, UINT16 value);

#endif
