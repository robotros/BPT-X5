/*******************************************************************************
* Title: SPI_Setup.h                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that allow the user to set up the SPI and   
* send and receive data.                                                       
*******************************************************************************/

#ifndef SPI_SETUP_H
#define SPI_SETUP_H

/*************************************************
*                  Constants                     *
*************************************************/ 
// SPI mode definitions
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H)
#define SPI_MODE_1  (SPI_L_TO_H)
#define SPI_MODE_2  (SPI_H_TO_L)
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 

/*************************************************
*                    Macros                      *
*************************************************/ 
#define SPI1_STATE(x)     (SPI1STATbits.SPIEN = x)
#define SPI2_STATE(x)     (SPI2STATbits.SPIEN = x)

/*************************************************
*              Function Prototypes               *
*************************************************/  
void SPI1_Init(void);  
void SPI2_Init(void);       
UINT16 SPI1_Send(UINT16 tx);    
UINT16 SPI2_Send(UINT16 tx);              
         
#endif              
