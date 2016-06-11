/*******************************************************************************
* Title: UART_Setup.c                                                          
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                            
*                                                                               
* Description:                                                                  
* This function will initialize the USART peripheral according to the values    
* that are passed into this function. To calculate baudrate, the following      
* formulas may be used:                                                         
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

#ifndef UART_SETUP_C
#define UART_SETUP_C

#include "Main_Includes.h"
#include "RS232_Setup.h"

/*******************************************************************************
* Function: UART_Init(UINT8 Tx, UINT8 Rx, UINT32 baudrate)                       
*                                                                               
* Variables:                                                                    
* Tx -> The value that is written to TXSTA to initialize the UART               
* Rx -> The value that is written to RCSTA to initialize the UART                
* baudrate -> The value for the desired baudrate                   
*                                                                               
* Description:                                                                  
* The user must input their desired UART conditions and this function will       
* set up the hardware UART in accordance to their specifications.               
*******************************************************************************/
void UART_Init(UINT32 baudrate)
{  
  RPINR18bits.U1RXR	 =	41;	 //UART1 receive set to RB9
  RPOR5bits.RP54R	 =	1;	 //UART1 transmit set to RC6
  
  U1BRG = (FCYC / (baudrate * 16)) - 1;	//set baud speed
  U1MODE	=	0x8000;	 //turn on module
  U1STA	=	0x8400;	 //set interrupts
  //reset RX interrupt flag
  IFS0bits.U1RXIF = 0;
  _U1RXIP = 7;
  _U1RXIE = 1;
}

/*******************************************************************************
* Function: UART_Init(UINT32 baudrate)                       
*                                                                               
* Variables:                                                                      
* baudrate -> The value for the desired baudrate                   
*                                                                               
* Description:                                                                  
* This function will change the baudrate of the UART.            
*******************************************************************************/
void UART_Set_Baudrate(UINT32 baudrate)
{  
  U1BRG = (FCYC / (baudrate * 16)) - 1;	//set baud speed
}

/*******************************************************************************
* Function: UART_Tx(UINT8 data)                                                 
*                                                                               
* Variables:                                                                    
* data -> Stores the data that is to be transmitted through the UART             
*                                                                               
* Description:                                                                  
* This function will accept a UINT8 of data and transmit it out of the hardware   
* UART module.                                                                  
*******************************************************************************/
void UART_Tx(UINT8 data)
{
  //transmit ONLY if TX buffer is empty
  while (U1STAbits.UTXBF == 1);
  U1TXREG = data;
}

/*******************************************************************************
* Function: UART_Rx(void)                                                       
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This function will check to see if any UART data has been received. If there   
* is data received, store it in the variable data and return it. If there is    
* no received data, the function will return a null character (0).              
*******************************************************************************/
UINT8 UART_Rx(void)
{
  char Temp;
  
  //Check if any UART data has been received
  if(U1STAbits.URXDA == 1)
    Temp = U1RXREG;
  
  else
  	Temp = 0;
  
  //return my received UINT8
  return Temp;
}

#endif
