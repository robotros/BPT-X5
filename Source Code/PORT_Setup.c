/*******************************************************************************
* Title: PORT_Setup.c                                                           
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                        
*                                                                               
* Description:                                                                  
* This file contains the functions that are used for setting up and             
* initializing the ports and register.                                          
*******************************************************************************/

#ifndef PORT_SETUP_C
#define PORT_SETUP_C

#include "Main_Includes.h"
#include "Interrupts.h"
#include "PORT_Setup.h"
#include "SPI_Setup.h"
#include "ADC_Setup.h"

extern volatile T16_FLAG FLAG1;

/*******************************************************************************
* Function: PORT_Init()                                                          
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Pinouts:                                                                      
*                                                                               
* Description:                                                                  
* This routine will initialize the ports as needed according to the schematic.  
* This should only be called once in a program.                                 
*******************************************************************************/
void PORT_Init(void)
{ 
 //Enable AN0 - AN3 as analog inputs
 ANSELA = 0x0003;
 ANSELB = 0x0003;
 
 //Default output settings for each pin
 LATA = 0x0480; 
 LATB = 0x1120; 
 LATC = 0x0000; 
 LATD = 0x0040; 
 LATE = 0xF000; 
 LATF = 0x0011; 
 LATG = 0x0000; 

 //Default TRIS settings for each pin
 TRISA = 0X0003;
 TRISB = 0X1383;
 TRISC = 0X0480;
 TRISD = 0X0000;
 TRISE = 0X0000;
 TRISF = 0X0000;
 TRISG = 0X0000; 
 
 //Activate internal pullup resistor for IR receiver line
 _CNPUB7 = 1;
 
  //Set up SPI2 ports: MOSI2 - RP43 ; SCK2 - RP42; MISO - RPI44
  RPOR4bits.RP43R = 0x08;
  RPOR4bits.RP42R = 0x09;
  RPINR22bits.SDI2R = 44;
}

/*******************************************************************************
* Function: Oscillator_Init(void)                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This function initializes the oscillator for 70 MHz operation.               
*******************************************************************************/
void Oscillator_Init(void)
{
  // Configure Oscillator to operate the device at 70Mhz
  // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
  // Fosc= 8M*70/(2*2)=140Mhz for 8M input clock
  PLLFBD = 68;            // M=70
  CLKDIVbits.PLLPOST = 0; // N1=2
  CLKDIVbits.PLLPRE = 0;  // N2=2
  OSCTUN = 0;             // Tune FRC oscillator, if FRC is used
  RCONbits.SWDTEN = 0;    /* Disable Watch Dog Timer*/

  //Clock switch to incorporate PLL
  __builtin_write_OSCCONH( 0x03 );            // Initiate Clock Switch to Primary

  //Oscillator with PLL (NOSC=0b011)
  __builtin_write_OSCCONL( OSCCON || 0x01 );  // Start clock switching
  while( OSCCONbits.COSC != 0b011 );

  // Wait for Clock switch to occur
  while( OSCCONbits.LOCK != 1 );
}

/*******************************************************************************
* Function: Modules_Init(void)                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This function will initialize various perihperals and is ran before the main
* loop.
*******************************************************************************/
void Modules_Init(void)
{
	//Clear all flags
  FLAG1.all = 0x0000;
  
  //Initialize External Interrupt #0
  //INT0_Init(ON,INT0_NEGATIVE_EDGE,INT_PRIORITY6);
  
  //Activate the SPI2 Bus
  SPI2_Init();
  
  //Activate the SPI1 bus for the LED grid
  SPI1_Init();
  
  //Enable the ADC module
  ADC_Init();
  
  //Timer 4 is used with the input capture module to decode IR signals
  TMR4_Init();
  
  //Enable the CCP module
// Initialize Capture Module
IC1CON1bits.ICM=0b00; // Disable Input Capture 1 module
IC1CON1bits.ICTSEL= 2; // Select Timer2 as the IC1 Time base
IC1CON1bits.ICI= 0b00; // Interrupt on every second capture event
IC1CON1bits.ICM= 0b010; // Generate capture event on every Rising edge
// Enable Capture Interrupt And Timer2
_IC1IP = 7; // Setup IC1 interrupt priority level
_IC1IF = 0; // Clear IC1 Interrupt Status Flag
_IC1IE = 1;

  _IC1IE = 1;
  
  RPINR7bits.IC1R = 39;
  
  //Enable Interrupts
  ENABLE_GLOBAL_INTERRUPTS();
}

#endif
