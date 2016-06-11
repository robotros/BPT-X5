/*******************************************************************************
* Title: Configurations.h                                                                        
* Version: 1.0                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                          
*                                                                               
* Description:                                                                  
* This file contains the various start-up config settings for the PIC MCU.
*******************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*************************************************\
*             Configuration Settings             *\
*************************************************/
#pragma config ICS = PGD1        // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF      // JTAG Enable bit (JTAG is disabled)\par

// FPOR\par
#pragma config ALTI2C1 = OFF     // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF     // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)

// FWDT\par
#pragma config WDTPOST = PS32768 // Watchdog Timer Postscaler bits (1:32,768)\par
#pragma config WDTPRE = PR128    // Watchdog Timer Prescaler bit (1:128)\par
#pragma config PLLKEN = ON       // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF      // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF      // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC\par
#pragma config POSCMD = XT       // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF    // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF     // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD    // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL\par
#pragma config FNOSC = FRC       // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = OFF        // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
#pragma config PWMLOCK = OFF

// FGS\par
#pragma config GWRP = OFF        // General Segment Write-Protect bit (General Segment may be written)

#endif
