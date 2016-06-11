/*******************************************************************************
* Title: TLC5955_Setup.h                                                       
* Version: 1.0                                                                    
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                       
*                                                                              
* Description:                                                                 
* This file contains the function prototypes and definitions that are used to                                                                             
* control the operation of the TLC5955 16-bit PWM driver.                                                                                                                            
*******************************************************************************/
    
#ifndef TLC5955_SETUP_H
#define TLC5955_SETUP_H  

/*************************************************
*                   Constants                    *
*************************************************/ 
#define TLC_MISO                  PORTBbits.RB12
#define TLC_MOSI                  _LATB11
#define TLC_SCK                   _LATB10
          
#define TLC_LAT                   _LATC11
#define TLC_ENA                   _LATE12
          
#define TLC5955_UPDATE            FLAG1.b0

#define TLC_CHIPS                 2

/************* TLC5955 Write Mode **************/
#define TLC5955_GS_MODE           0
#define TLC5955_CONTROL_MODE      1

/****** Function Control Data Latch Bits *******/
//Auto Display Repeat Mode
#define TLC5955_DSPRPT_ON          0x01 
#define TLC5955_DSPRPT_OFF         0x00    

//Display Timing Reset Mode           
#define TLC5955_TMGRST_ON         0x02 
#define TLC5955_TMGRST_OFF        0x00 

//Auto Data Refresh Mode                           
#define TLC5955_RFRESH_ON         0x04 
#define TLC5955_RFRESH_OFF        0x00 

//ES-PWM Mode                             
#define TLC5955_ESPWM_ON          0x08 
#define TLC5955_ESPWM_OFF         0x00  

//LSD Detection Voltage Select                         
#define TLC5955_LSDVLT_ON         0x10 
#define TLC5955_LSDVLT_OFF        0x00                     

/*************************************************
*                   Macros                       *
*************************************************/
#define FAST_SPI(x)       (SPI2BUF = x)
#define SPI2_STATE(x)     (SPI2STATbits.SPIEN = x)
#define PULSE(x)          {x = 1; x = 0;}

/*************************************************
*              Function Prototypes               *
*************************************************/ 
void TLC5955_Update(void); 
void TLC5955_Init(UINT8 FC_data, UINT8 *DC_data, UINT16 MC_data, UINT32 BC_data);
void TLC5955_Default_Init(UINT8 FC_data);
void XLAT_Interrupt(void);
void Set_Grayscale(void);
void Set_Initial_Grayscale(void);
void Dot_Correction(void);      
void Write_Single_Bit(UINT8 state);
void TLC5955_Write_GS(UINT16 *data);

#endif
