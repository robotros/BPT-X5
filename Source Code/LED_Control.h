/*******************************************************************************
* Title: LED_Control.h                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the various function prototypes and definitions that are used                                                                              
* control the various features on the X5 PCB.                                                                                                                            
*******************************************************************************/
    
#ifndef LED_CONTROL_H
#define LED_CONTROL_H   

/*************************************************
*                   Constants                    *
*************************************************/
//This will be set if either of the ball washers are active
#define BW_ACTIVE       FLAG1.b6

//Alerts the program to display an error message
#define DIAGNOSE_ERROR  FLAG1.b7

//Either of these will be set if any of the ball washers 'jam' (The ball is stuck)
#define BW1_JAM         FLAG1.b8
#define BW2_JAM         FLAG1.b9

//Used to set the PIC into standy mode
#define MODE_STANDBY      FLAG1.b11

//LEDx Channels on PCB - Channel 12 - 27 on TLC5955 #2
#define LED1        			60
#define LED2        			61
#define LED3        			62
#define LED4        			63
#define LED5        			64
#define LED6        			65
#define LED7        			66
#define LED8        			67
#define LED9        			68
#define LED10       			69
#define LED11       			70 
#define LED12       			71 
#define LED13       			72 
#define LED14       			73 
#define LED15       			74 
#define LED16       			75

//LED Ring definitions (Only used with Update_Ring(a,b) function)
//If you choose to use the Update_Channel(a,b) function to update
//the LED rings, make sure to use the LEDx definitions above as
//those are the actual channel locations of the LED rings.
#define LED_RING1          1
#define LED_RING2          2
#define LED_RING3          3
#define LED_RING4          4
#define LED_RING5          5
#define LED_RING6          6
#define LED_RING7          7
#define LED_RING8          8
#define LED_RING9          9
#define LED_RING10        10
#define LED_RING11        11
#define LED_RING12        12
#define LED_RING13        13
#define LED_RING14        14
#define LED_RING15        15
#define LED_RING16        16

//Motor Controllers - Channel 28 - 31 on TLC5955 #2
#define BW1_FAN           76
#define BW1_PUMP          77
#define BW2_FAN           78
#define BW2_PUMP          79

//IR Transmitter Driver - Channel 32 on TLC5955 #2
#define IR_TX             80

//Extra PWM Outputs - Channel 33 - 44 on TLC5955 #2
// These pins require a 2x7 IDE Breakout Connector 
#define EXTRA_PWM1        81 
#define EXTRA_PWM2        82
#define EXTRA_PWM3        83
#define EXTRA_PWM4        84
#define EXTRA_PWM5        85
#define EXTRA_PWM6        86
#define EXTRA_PWM7        87
#define EXTRA_PWM8        88
#define EXTRA_PWM9        89
#define EXTRA_PWM10       90
#define EXTRA_PWM11       91 
#define EXTRA_PWM12       92 

//Used to adjust all of the LED rings at once. All LED rings
//includes the ball washer LED rings.
#define ALL_LED_RINGS       1
#define MAIN_LED_RINGS      0

//Ball washers #1 and #2
#define BW1                 1
#define BW2                 2

//Ball washer REGULAR pump and fan speeds
#define BW1_PUMP_SPEED      65535
#define BW2_PUMP_SPEED      65535
#define BW1_FAN_SPEED       65535
#define BW2_FAN_SPEED       65535

//Ball washer LOW fan speeds, used to detect
//if the ball has been grabbed from the exit hole.
//If it hasn't, this speed will lower the ball back
//down the hole where it will trip the sensor.
#define BW1_FAN_LOW_SPEED  65535
#define BW2_FAN_LOW_SPEED  65535

//Disable Ball Washer Motors
#define BW1_PUMP_OFF        0
#define BW2_PUMP_OFF        0
#define BW1_FAN_OFF         0
#define BW2_FAN_OFF         0

//The amount of times that the ball washer will try to
//blow a stuck ball out of the ball washer before it
//times out and sets an error flag
#define BW_REPEATS          3

//Ball washer IR sensor bits
#define BW1_ENTRY           0x01
#define BW1_EXIT            0x02
#define BW2_ENTRY           0x04
#define BW2_EXIT            0x08

//Ball Washer Error Codes
#define BW1_JAM_ERROR_CODE  0xFE
#define BW2_JAM_ERROR_CODE  0xFD

//RGB Pod Brightness (1 - 20)
#define POD_BRIGHTNESS_MAX  20
#define POD_BRIGHTNESS_MIN  1

/*************************************************
*                   Macros                       *
*************************************************/
#define TLC_SET_CHANNEL(c,d)     (TLC_data[c] = d)  

/*************************************************
*              Function Prototypes               *
*************************************************/    
void Fill_Grid(void);     
void Clear_Grid(void); 
void Fade_State(void);
void Set_All_Pods(RGB color);   
void Disable_All_Features(void);
void RGB_Underlighting(RGB underlight);
void Pod_Set_Color(UINT8 pod, RGB pod_color);
void Adjust_Pod_Brightness(UINT8 *brightness);
void Fade_All_Pods(RGB NEW, UINT16 fade_rate);
void Update_Ring(UINT8 ring, UINT16 duty_cycle);
void Fade_Pod(UINT8 pod, RGB NEW, UINT16 delay);
void Set_All_Rings(UINT16 duty_cycle, UINT8 mode);
void Fade_Ring(UINT8 LED, float new, UINT16 delay);
void Update_Channel(UINT16 channel, UINT16 duty_cycle);
void RGB_Pod(UINT8 pod, UINT16 red, UINT16 green, UINT16 blue);
void Fade_All_Rings(UINT16 duty_cycle, UINT16 fade_rate, UINT8 mode);

#endif
