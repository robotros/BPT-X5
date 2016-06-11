/*******************************************************************************
* Title: ADC_Setup.h                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that are needed in order to initialize  
* and control the operation of the ADC module.                                 
*******************************************************************************/

#ifndef ADC_SETUP_H
#define ADC_SETUP_H

/*************************************************
*                  Constants                     *
*************************************************/ 
#define CHANNEL_AN0       0
#define CHANNEL_AN1       1
#define CHANNEL_AN2       2
#define CHANNEL_AN3       3
#define CHANNEL_AN4       4
#define CHANNEL_AN5       5
#define CHANNEL_AN6       6
#define CHANNEL_AN7       7
#define CHANNEL_AN8       8
#define CHANNEL_AN9       9
#define CHANNEL_AN10      10 
#define CHANNEL_AN11      11
#define CHANNEL_AN12      12
#define CHANNEL_AN13      13
#define CHANNEL_AN14      14
#define CHANNEL_AN15      15
#define CHANNEL_AN16      16
#define CHANNEL_AN17      17
#define CHANNEL_AN18      18
#define CHANNEL_AN19      19
#define CHANNEL_AN20      20
#define CHANNEL_AN21      21
#define CHANNEL_AN22      22
#define CHANNEL_AN23      23
#define CHANNEL_AN24      24
#define CHANNEL_AN25      25
#define CHANNEL_AN26      26
#define CHANNEL_AN27      27
#define CHANNEL_AN28      28
#define CHANNEL_AN29      29
#define CHANNEL_AN30      30
#define CHANNEL_AN31      31

/*************************************************
*                   Macros                       *
*************************************************/
#define ADC_CHANNEL(x)      (AD1CHS0 = x)

/*************************************************
*              Function Prototypes               *
*************************************************/  
void ADC_Init(void);     
void ADC_Read_SS(UINT16 *buf);

UINT16 ADC_Read(void);   
   
#endif
