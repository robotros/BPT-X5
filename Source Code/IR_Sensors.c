/*******************************************************************************
* Title: IR_Sensors.c                                                                      
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                           
* Date: March 13, 2015                                                                        
*                                                                              
* Description:                                                                 
* This file containts the functions that are used to calibrate and read the IR                                                                             
* sensors on the beer pong table. This allows us to detect when a cup has been                                                                             
* removed from or placed on a pod, and it allows us to detect when a ball has
* been dropped into either of the ballwashers.     
*  
* In accordance with the schematic, each side of the BPT has its own dedicated ADC
* port for its 74HC4051 chips. Chips #1 connects to AN0, chip #2 connects to AN1 and
* chip #3 connects to AN2.
*
* IR_DRV controls the operation of the IR transmitter and should be used in conjuntion
* with TLC_SET_CHANNEL(a,b) in LED_Control.h.   
*                      
* To save time, the ADC module is set up to sample four inputs (AN0 - AN3)
* simultaneously. AN3 is used for the VU meter module and does not affect the operation
* of the IR sensors, although it does get read with them. 
*                                                                    
* The IR sensor values will be stored in a 24-byte variable array as follows:
*
* Bytes:
*
*     1st 10x RGB Pods (Master Side)
* ----------------------------------
*  0 -> RGB Pod #1  - IR Sensor #1  
*  1 -> RGB Pod #2  - IR Sensor #2  
*  2 -> RGB Pod #3  - IR Sensor #3  
*  3 -> RGB Pod #4  - IR Sensor #4  
*  4 -> RGB Pod #5  - IR Sensor #5  
*  5 -> RGB Pod #6  - IR Sensor #6  
*  6 -> RGB Pod #7  - IR Sensor #7  
*  7 -> RGB Pod #8  - IR Sensor #8  
*  8 -> RGB Pod #9  - IR Sensor #9  
*  9 -> RGB Pod #10 - IR Sensor #10    
*
*      2nd 10x RGB Pods (Secondary Side)
* --------------------------------------
*  10 -> RGB Pod #11  - IR Sensor #11  
*  11 -> RGB Pod #12  - IR Sensor #12  
*  12 -> RGB Pod #13  - IR Sensor #13  
*  13 -> RGB Pod #14  - IR Sensor #14  
*  14 -> RGB Pod #15  - IR Sensor #15  
*  15 -> RGB Pod #16  - IR Sensor #16  
*  16 -> RGB Pod #17  - IR Sensor #17  
*  17 -> RGB Pod #18  - IR Sensor #18  
*  18 -> RGB Pod #19  - IR Sensor #19  
*  19 -> RGB Pod #20  - IR Sensor #20 
*
*      Ball Washer IR Sensors (Master Side)
* -----------------------------------------
*  20 -> IR Ballwasher #1 Entry 
*  21 -> IR Ballwasher #1 Exit   
*
*      Ball Washer IR Sensors (Secondary Side)
* --------------------------------------------
*  22 -> IR Ballwasher #2 Entry 
*  23 -> IR Ballwasher #2 Exit   
*                                                                                                                         
*******************************************************************************/

#ifndef IR_SENSORS_C
#define IR_SENSORS_C

#include "Main_Includes.h"
#include "IR_Sensors.h"
#include "ADC_Setup.h"
#include "LED_Control.h"
#include "Delay_Setup.h"
#include "TLC5955_Setup.h"
#include <stdio.h>

/*************************************************
*               Global Variables                 *
*************************************************/
extern volatile INT16 cal_light[24];

extern volatile UINT16 IR_duty;
extern volatile UINT16 IR_value[24];
extern volatile UINT16 TLC_data[96];

extern volatile UINT32 sensor_bits;

extern volatile RGB COLOR[11];

/*******************************************************************************
* Function: Sensor_Calibration(void)                                                                    
*                                                                              
* Variables:                                                                   
* IR_cal[24] -> All calibration analog values stored in this global UINT16 array                                                                          
*                                                                              
* Description:                                                                 
* This function will calibrate the IR sensors by reading each sensor 'n' amount of                                                                            
* times and calculating the average reading. The amount of times that each sensor                                                                             
* be read is determined by CAL_DIV located in the header file. Since average is a
* 16-bit variable, CAL_DIV should not go above the value of 64.
* 
* Max ADC Value = 1023
*                  
* average = 1023 * CAL_DIV_MAX (64)
*         = 65472
*                  
* If CAL_DIV is any higher than 64 and your ADC values are at max value for all of 
* the readings, you will overflow the 16-bit 'average' variable. If you need more
* than 64 readings to average, you can change 'average' to a 32-bit varaiable.                                                                            
*******************************************************************************/
void Sensor_Calibration(void)
{
  UINT8 i,j,k;
  UINT8 tracker;
    
  UINT16 adc_value[4];
  
  //Turn on the IR transmitters 
  Set_IR_PWM(65535); 
  ADC_CHANNEL(3);
  
  //Turn on all pods to their max brightness before calibration
  Set_All_Pods(COLOR[WHITE]);
  Delay_ms(50);
  
  //Reset each calibrated value to 0
  for (i = 0;i < 24;i++)
    cal_light[i] = 0; 
  
  //Begin reading each sensors value multiple times
  for (k = 0;k < CAL_DIV;k++)
  { 
	  //There are 3x 75HC4051 ICs, each with 8 inputs. Read each input. 
    for (j = 0;j < 8;j++)
    {  
	    //Read analog inputs AN0 - AN3  
      ADC_Read_SS(adc_value);  
      Delay_us(10); 
      
      //Update the 74HC4051's to read the next input
      IR_CHANNEL(j+1);
      
      //Seperate each analog reading into its respective place in the sensor array
      for (i = 0;i < 3;i++)
      { 
	      //Calculate which sensor the analog reading corresponds to 
        tracker = j + (i*8);
        
        //Keeping adding up each read which will allow us to average the readings
        cal_light[tracker] += adc_value[i];
      }    
    } 
    
    //Allow a couple of milliseconds between each read to settle the inputs
    //This really isn't neccessary and is just here to ensure solid readings
    Delay_ms(2);
  }  
  
  //Average the readings of each sensor   
  for (i = 0;i < 24;i++)
    cal_light[i] /= CAL_DIV;
  
  //Send the calibrated readings out the UART module, this is nice for debugging
  for (i = 0;i < 24;i++)   
      printf("Light [%u]: %u\r\n",i,cal_light[i]);
  
  //Turn off the pods after calibration
  Set_All_Pods(COLOR[BLACK]);
}	

/*******************************************************************************
* Function: Update_Sensors(void)                                                                  
*                                                                              
* Variables:                                                                   
* N/A                                                                         
*                                                                              
* Description:                                                                 
* This function will update the value of the IR sensors which can then be compared
* to the calibrated values to see if a detection has been made (cup moved, ball
* put in washer, etc.). This function will return a 32-bit integer where each
* bit represents one of the IR sensors. The format is as follow:
*
*   Bit#  |  Sensor
*     0  -> RGB Pod #1
*     1  -> RGB Pod #2
*     2  -> RGB Pod #3
*     3  -> RGB Pod #4
*     4  -> RGB Pod #5
*     5  -> RGB Pod #6
*     6  -> RGB Pod #7
*     7  -> RGB Pod #8
*     8  -> RGB Pod #9
*     9  -> RGB Pod #10
*    10  -> RGB Pod #11
*    11  -> RGB Pod #12
*    12  -> RGB Pod #13
*    13  -> RGB Pod #14
*    14  -> RGB Pod #15
*    15  -> RGB Pod #16
*    16  -> RGB Pod #17
*    17  -> RGB Pod #18
*    18  -> RGB Pod #19
*    19  -> RGB Pod #20
*    20  -> BW1 Entry Sensor
*    21  -> BW1 Exit Sensor
*    22  -> BW2 Entry Sensor
*    23  -> BW2 Exit Sensor
*    24  -> Not Used
*     .  -> .
*     .  -> .
*    31  -> Not Used
*                                                                        
*******************************************************************************/
UINT32 Update_All_Sensors(void)
{ 
  UINT8 i,j;
  UINT8 tracker;
  
  INT16 adc_value[4];
  
  UINT32 data = 0;
  static UINT32 last_data[5] = {0,0,0,0,0};
  
  //Prepare to start simultaneously reading channels AN0 - AN3
  ADC_CHANNEL(3);
  
  //Prepare to sample all of the input channels on each 74HC4051
  for (j = 0;j < 8;j++)
  {    	    
	  //Read analog inputs AN0 - AN3  
    ADC_Read_SS(adc_value);  
      
    //Update the 74HC4051's to read the next input
    IR_CHANNEL(j+1);
    
	  //There are 3x 75HC4051 ICs, each with 8 inputs. Read each input. 
    for (i = 0;i < 3;i++)
    {   
	    //Calculate which sensor the analog reading corresponds to 
      tracker = j + (i*8);
      
      //Store each sensors most current light reading in a global array
      //This allows us to retrieve these values when testing the sensors in the PC app
      IR_value[tracker] = adc_value[i];
      
      //If the received ADC value is much larger than the calibrated value,
      //an object has been detected. Set the bit to 1.
      if ((adc_value[i] - cal_light[tracker]) > 110)      
        data |= ((UINT32)1 << tracker); 
      
      //No object has been detected, clear the bit to a 0  
      else
        data &= ~((UINT32)1 << tracker);  
    }    
  } 
  
  //This is a really basic form of error checking. It is needed for heavily
  //frosted acrylic sheets where lots of the IR light is reflected back into
  //the receiver. Set the IR sensitivty low enough that it never false detects
  //a cup when there isn't one over top of the pod. This way, when you do set a
  //cup over top of the pod, it may have the odd false detection where it may not
  //'see' the cup on top of it but the last 4 readings will tell it otherwise.
  //Unless it has 5 false detections in a row (which I haven't had happen), it
  //won't change the detection rate of the sensor. This is a very basic way of error
  //checking, but it seems to work great. If you are using a clear acrylic sheet, you
  //can take it out and won't need it. Anything frosted, it's a good idea to keep it.
  last_data[4] = last_data[3];
  last_data[3] = last_data[2];
  last_data[2] = last_data[1];
  last_data[1] = last_data[0];
  last_data[0] = data;
  
  //OR all of the last 5 readings together. If there is a false detection in the
  //bunch where it thinks no cup is above the sensor and it is, the other 4
  //readings will adjust it.
  data = last_data[0] | last_data[1] | last_data[2] | last_data[3] | last_data[4];
  
  //Return the sensor data
  return data; 
}  

/*******************************************************************************
* Function: Enable_IR_Sensors(UINT16 duty)                                                                    
*                                                                              
* Variables:                                                                   
* duty -> Adjusts the brightness of the IR transmitters on the sensor modules                                                                          
*                                                                              
* Description:                                                                 
* This function will enable the 74HC4051 multiplexers and also set the brightness
* of the IR transmitters on the sensor modules.                                                                             
*******************************************************************************/
void Enable_IR_Sensors(UINT16 duty)
{
	//Enable all three 74HC4051 ICs
  LATE &= 0x1FFF;
  
  //Set the IR transmitters brightness
  Set_IR_PWM(duty);
}  

/*******************************************************************************
* Function: Set_IR_PWM(UINT16 duty_cycle)                                                                    
*                                                                              
* Variables:   
* duty_cycle -> Adjusts the brightness of the IR transmitters on the sensor modules                                                                 
*                                                                              
* Description:                                                                 
* This function allows the user to set the brightness of the IR transmitters that
* are contained in the TCRT5000 IR Sensor modules.                                                                             
*******************************************************************************/
void Set_IR_PWM(UINT16 duty_cycle)
{
  //Save modified data in appropriate TLC5955 register
  TLC_data[IR_DRV] = duty_cycle; 
  
  //Update the new LED ring data
  IR_duty = duty_cycle;
  
  //Call for an update
  TLC5955_Update();
} 
    
#endif 
