/*******************************************************************************
* Title: IR_Sensors.h                                                                      
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file containts the functions that are used to calibrate and read the IR                                                                             
* sensors on the beer pong table. This allows us to detect when a cup has been                                                                             
* removed from or placed on a pod and it allows us to detect when a ball has
* been dropped into either of the ballwashers.                                                                             
*******************************************************************************/

#ifndef IR_SENSORS_H
#define IR_SENSORS_H

/*************************************************
*                   Constants                    *
*************************************************/
//Channel 80 of the two cascaded TLC5955 chips controls the IR transmitters
#define IR_DRV      80      

//The maximum brightness value for the IR transmitters on the sensors
#define TX_MAX_BRIGHTNESS		65535    

//Chip selects for each of the 74HC4051 8-channel multiplexers
//I just set these on and off by bitmasking PORTE
//#define IR1_EN      _LATE13
//#define IR2_EN      _LATE14
//#define IR3_EN      _LATE15

//Pin selects for the (A = LSb <-> C = MSb)
#define IR_S0       _LATC0
#define IR_S1       _LATC1
#define IR_S2       _LATC2

//This adjusts the sensitivity of the IR sensors. Higher Value = Less Sensitive,
//Lower Value = More Sensitive (Value has to be between 0 and 1023 of course)
//BW_THRESHOLD is for the ball washers which we want to be a little more sensitive
#define IR_THRESHOLD        400
#define BW_THRESHOLD        150

//Determines the # of times each sensor will be read and averaged with.
//Do not go higher than 64. Explained in more detail in the 'C' file.
#define CAL_DIV       30

//The delay period in uS after each IR sensor ADC reading; If you are getting
//inconsistent values try increasing this.
#define IR_DELAY      20

/*************************************************
*                   Macros                       *
*************************************************/
#define IR_CHANNEL(x)      {LATC &= 0xFFF8; LATC |= ((x) & 0x0007);} 
#define IR_INPUTS(x)       {IR1_EN = x; IR2_EN = x; IR3_EN = x;}

/*************************************************
*              Function Prototypes               *
*************************************************/
UINT32 Update_All_Sensors(void);
void Enable_IR_Sensors(UINT16 duty);

void Sensor_Calibration(void);
void Set_IR_PWM(UINT16 duty_cycle);

#endif
