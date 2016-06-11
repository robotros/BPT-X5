/*******************************************************************************
* Title: UART_Setup.h                                                             
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                              
*                                                                               
* Description:                                                                  
* This file will initialize the USART peripheral according to the values that   
* are passed into this function. To calculate baudrate, the following formulas  
* may be used:                                                                  
*                                                                               
* SPBRG:SPBRGH = (((Fosc / Desired_Baudrate) / (64 or 16 or 4)) - 1)            
*                                                                                
* Actual_Baudrate = (Fosc / ((64 or 16 or 4) * (SPBRG + 1)))                    
*                                                                               
* %Error = ((Calculated_Baudrate - Desired_Baudrate) / Desired_Baudrate)        
*                                                                               
*      	 TXSTA   RCSTA   BAUDCON                                                
*      	 =====   =====   =======                                                 
* Bit7:	 CSRC    SPEN    ABDOVF                                                  
* Bit6:	 TX9     RX9     RCIDL                                                   
* Bit5:	 TXEN    SREN    RXDTP                                                   
* Bit4:	 SYNC    CREN    TXCKP                                                   
* Bit3:	 SENDB   ADDEN   BRG16                                                   
* Bit2:	 BRGH    FERRN     -                                                     
* Bit1:	 TRMT    OERR    WUE                                                     
* Bit0:	 TX9D    RX9D    ABDEN                                                  
*                                                                               
*******************************************************************************/

#ifndef UART_SETUP_H
#define UART_SETUP_H

/*************************************************
*                  Constants                     *
*************************************************/ 

/*************************************************
*                   Macros                       *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/   
void UART_Tx(UINT8 data);
void UART_Init(UINT32 baudrate);
void UART_Set_Baudrate(UINT32 baudrate);

UINT8 UART_Rx(void);

UINT16 UART_Calculate_Baudrate(UINT32 baudrate);

#endif
