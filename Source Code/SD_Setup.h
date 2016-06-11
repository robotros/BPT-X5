/*******************************************************************************
* Title: SD_Setup.h                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                           
*                                                                              
* Description:                                                                 
* This file contains the constants and function prototypes that are used in    
* initializing and controlling an SD Card.                                  
*******************************************************************************/

#ifndef SD_SETUP_H
#define SD_SETUP_H

/*************************************************
*               Pin Designations                 *
*************************************************/ 
#define SD_CS            _LATA10   
#define SD_SCK           _LAT10
#define SD_MOSI          _LATB11
#define SD_MISO          PORTBbits.RB12

/*************************************************
*                  Constants                     *
*************************************************/ 
//Used in SD_Match(x) 
#define SUCCESSFUL_MATCH        0x00
#define NO_MATCH                0xA1

//Used in SD_Busy()
#define SD_BUSY_TIMEOUT         0xA2
#define SD_CARD_IDLE            0x00

//Used in SD_Init()
#define SD_SUCCESS              0x00
#define SD_RESET_ERROR          0xA3
#define SD_INIT_ERROR           0xA4
#define SD_BLOCKLEN_ERROR       0xA5

//Used in SD read routines
#define SD_READ_ERROR           0xA6

//Used in SD write routines
#define SD_CMD24_ERROR          0xA7
#define SD_CMD25_ERROR          0xA8
#define SD_WRITE_ERROR          0xA9

//Used in SD_Erase
#define SD_START_ERASE_ERROR    0xAA
#define SD_END_ERASE_ERROR      0xAB
#define SD_ERASE_ERROR          0xAC

//Used in SD_Command(a,b)
#define SD_CMD_LEN               0x06
#define SD_CMD_REPEAT            10000

//Used in SD_Capacity()
#define SD_CSD_ERROR             0xAD

//The default sector size of any SD card
#define SD_SECTOR_SIZE           0x0200

//The size of the CSD response (16 bytes)
#define SD_CSD_SIZE               16

//Definitions for MMC/SDC command
#define SD_CMD0	  (0x40+0)	//GO_IDLE_STATE 
#define SD_CMD1	  (0x40+1)	//SEND_OP_COND (MMC) 
#define	SD_ACMD41	(0xC0+41)	//SEND_OP_COND (SDC) 
#define SD_CMD8	  (0x40+8)	//SEND_IF_COND 
#define SD_CMD9	  (0x40+9)	//SEND_CSD 
#define SD_CMD10	(0x40+10)	//SEND_CID 
#define SD_CMD12	(0x40+12)	//STOP_TRANSMISSION 
#define SD_ACMD13	(0xC0+13)	//SD_STATUS (SDC) 
#define SD_CMD16	(0x40+16)	//SET_BLOCKLEN 
#define SD_CMD17	(0x40+17)	//READ_SINGLE_BLOCK 
#define SD_CMD18	(0x40+18)	//READ_MULTIPLE_BLOCK 
#define SD_CMD23	(0x40+23)	//SET_BLOCK_COUNT (MMC) 
#define	SD_ACMD23	(0xC0+23)	//SET_WR_BLK_ERASE_COUNT
#define SD_CMD24	(0x40+24)	//WRITE_BLOCK 
#define SD_CMD25	(0x40+25)	//WRITE_MULTIPLE_BLOCK 
#define SD_CMD32  (0x40+32)	//ERASE_WR_BLK_START_ADDR 
#define SD_CMD33  (0x40+33)	//ERASE_WR_BLK_END_ADDR  
#define SD_CMD38  (0x40+38)	//ERASE 
#define SD_CMD55	(0x40+55)	//APP_SD_CMD 
#define SD_CMD58	(0x40+58)	//READ_OCR 

/*************************************************
*                   Macros                       *
*************************************************/
//Set this to match the SPI send routine in your program
#define SPI_SD_SEND(x)						SPI2_Send(x)

#define SD_ENABLE()      (SD_CS = 0 )
#define SD_DISABLE()     {SD_CS = 1; SPI_SD_SEND(0xFF);}

/*************************************************
*              Function Prototypes               *
*************************************************/ 
UINT8 SD_Busy(void);
UINT8 SD_Init(void); 
UINT8 SD_Match(UINT8 check);
UINT8 SD_nMatch(UINT8 check);
UINT8 SD_Read_CSD(UINT8 *buf);
UINT8 SD_Command(UINT8 *data, UINT16 len);
UINT8 SD_Read_Sector(UINT32 sector, UINT8 *buf);
UINT8 SD_Write_Sector(UINT32 sector, UINT8 *buf);
UINT8 SD_Erase_Sectors(UINT32 start, UINT32 amount);
UINT8 SD_Read_Mul_Sectors(UINT32 start, UINT32 amount);
UINT8 SD_Write_Mul_Sectors(UINT32 start, UINT32 amount);

UINT32 SD_Capacity(void);

#endif
