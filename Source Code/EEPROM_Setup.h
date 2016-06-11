/*******************************************************************************
* Title: EEPROM_Setup.h                                                                       
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file contains the bit definitions and function prototypes required to                                                                             
* control the 25LCxxx/25AAxxx line of EEPROMs. The EEPROM capacity definition                                                                            
* and the page size definition must be modified to suit the specific EEPROM
* chip that you are using. By default, it is set for a 25LC256/25AA256 EEPROM
* from Microchip.                                                                             
*******************************************************************************/

#ifndef EEPROM_SETUP_H
#define EEPROM_SETUP_H

/*************************************************
*               Bit Definitions                  *
*************************************************/
#define EE_CS                 		_LATF0
#define EE_HOLD               		_LATF1
		
//SPI Definitions (Only needed if bit-banging SPI)      
#define EE_MOSI               		_LATB11
#define EE_MISO               		_PORTB12
#define EE_SCLK               		_LATB10

/*************************************************
*                   Constants                    *
*************************************************/
//Set this definition to your specfified SPI module
#define EE_SPI_Send(x)        		SPI2_Send(x)
   		
//EEPROM Command Set   		
#define EE_WRSR               		0x01   
#define EE_WRITE              		0x02      
#define EE_READ               		0x03       
#define EE_WRDI               		0x04        
#define EE_RDSR               		0x05     
#define EE_WREN               		0x06 
		
//EEPROM Max Capacity		
#define EEPROM_CAPACITY       		32768
		
//EEPROM Page Size in Bytes		
#define EEPROM_PAGE_SIZE      		64 
		
//Used in EEPROM_Write(a,b,c)		
#define EEPROM_CAPACITY_ERROR 		0x01
		
//Used in various functions to determine a successful operation
#define EEPROM_SUCCESS        		0x00

/*************************************************
*              Function Prototypes               *
*************************************************/
void EEPROM_Init(void);
void EEPROM_Command(UINT8 cmd);
void EEPROM_Read(UINT16 addr, UINT8 *buf, UINT16 len);

UINT8 EEPROM_Status(void);
UINT8 EEPROM_Write(UINT16 addr, UINT8 *buf, UINT16 len);

#endif
