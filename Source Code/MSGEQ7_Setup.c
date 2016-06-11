/*******************************************************************************
* Title: MSGEQ7_Setup.c                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that are used to control the VU Meter Attachment
* and read the seven different frequencies from the microphone or the AUX input.                                                                                 
*******************************************************************************/

#ifndef MSGEQ7_SETUP_C
#define MSGEQ7_SETUP_C

#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "MSGEQ7_Setup.h"
#include "ADC_Setup.h"
#include "LED_Control.h"

extern volatile UINT32 count32;
extern volatile RGB COLOR[11];

/*******************************************************************************
* Function: MSGEQ7_Init(void)                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This function will reset the MSGEQ7, allowing us to begin controlling it.
*******************************************************************************/
void MSGEQ7_Init(void)
{
	//Set to default idle state
  MSG_STROBE = 1;
  
  //Reset the MSGEQ7
  MSG_RESET = 1;
  Delay_us(1);
  MSG_RESET = 0;
}

/*******************************************************************************
* Function: MSGEQ7_Read(UINT16 *channel)                                                                     
*                                                                               
* Variables:                                                                    
* *channel -> Points to seven 16-bit variables to store the ADC readings
*                                                                               
* Description:                                                                  
* This function will save all seven ADC readings in a specified variable array.
* The order of the frequencies that are stored are:
*
* channel[0] -> 63Hz Band
* channel[1] -> 160Hz Band
* channel[2] -> 400Hz Band
* channel[3] -> 1kHz Band
* channel[4] -> 2.5kHz Band
* channel[5] -> 6.25kHz Band
* channel[6] -> 16kHz Band
*
*******************************************************************************/
void MSGEQ7_Read(UINT16 *channel)
{
  UINT8 i;
  UINT16 buf[4];
  
  //Strobe the MSGEQ7 seven times to read all seven frequency bands
  for (i = 0;i < 7;i++)
  {
	  //Set strobe to idle state and allow it enough time to settle
    MSG_STROBE = 0;
    Delay_us(80);
    
    //Read AN3 on the PIC, which is connected to the VU meter attachment
    ADC_Read_SS(buf);
    
    //Store the current bands reading
    channel[i] = buf[3];
    
    //Strobe the MSGEQ7 so that it will read the next frequency band on the next loop
    MSG_STROBE = 1;
    Delay_us(80);
  }  
}

/*******************************************************************************
* Function: MSGEQ7_Auto_Adjust(UINT16 *chan, UINT16 *level)                                                                     
*                                                                               
* Variables:                                                                    
* *chan -> Points to the 7 DWORD frequency band ADC values        
* *level -> Stores each frequency bands audio intensity on a scale of 0 - 31                                                                   
*                                                                               
* Description:                                                                  
* This function will take the raw ADC readings from the MSGEQ7, compensate for a
* specific offset with each band to remove noise, then calculate the intensity
* of each frequency, assigning it a value between 0 - 31, with 31 being the highest
* intensity.                                                                               
*******************************************************************************/
void MSGEQ7_Auto_Adjust(UINT16 *chan, UINT16 *level)
{
	static float max[7];
	static float min[7];

	UINT8 i;
	UINT16 diff;
	UINT16 raw_value;
	static UINT16 last_level[7] = {0,0,0,0,0,0,0};
	float step;
	
	//These offsets are defined is MSGEQ&_Setup.h
	UINT8 offset[7] = {CH0_OFFSET,CH1_OFFSET,CH2_OFFSET,CH3_OFFSET,   
										 CH4_OFFSET,CH5_OFFSET,CH6_OFFSET};
	
	//Cycle through each of the seven channels
	for (i = 0;i < 7;i++)
	{
		//Each time the channel value is less than max, lower the max threshold a bit
		if (chan[i] < max[i])
			max[i] *= 0.999;
		
		//A new max value has exceeded the old threshold, set the new value	
		else
			max[i] = chan[i];
		
		
		//Each time the channel value is higher than min, raise the min threshold a bit
		if (chan[i] > min[i])
			min[i] *= 1.01;
		
		//A lower value than the previous min threshold has been captured; Set new value	
		else
			min[i] = chan[i];
			
		if (max[i] < 100)
		  max[i] = 100;
			
		if (min[i] > 50)
		  min[i] = 50;
		
		//Mask off any values that could just be noise. This was set up through trial and error
		if ((chan[i] - min[i]) < offset[i])
				chan[i] = min[i];
	
		//Find the value between the max and min thresholds
		diff = max[i] - min[i];
		
		//Calculate out the ADC value per step (By default there are 32 steps calculated)
		step = (float)diff / VU_STEPS;
		
		//Get the amount of real data read from the ADC (Subtract off the min values)
		raw_value = chan[i] - min[i];
		
		//Calculate where the sound intesity falls between the allowed amount of steps (*VU_STEPS*)
		level[i] = raw_value / step;
		
		//Peak Hold each value; Instead of quickly switching between highs and lows, allow the value 
		//to decrease steadily, providing smooth transitions
		if (PEAK_HOLD)
		{
			//If the new value is less than the previous one, decrease the intensity by the amount of
			//steps that 'PEAK_LEVEL' is set to
			if (level[i] < last_level[i] && last_level[i] >= PEAK_LEVEL)
				level[i] = last_level[i] - PEAK_LEVEL;   
		}	
		
		//Save the current reading for the next loop through
		last_level[i] = level[i];	  
	}
}	
  
#endif