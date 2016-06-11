/*******************************************************************************
* Title: VU_Control.c                                                     
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the function and routines that are used to control the 
* animations that are used with the VU meter attachment.                                                                                                                                                         
*******************************************************************************/

#ifndef VU_CONTROL_C
#define VU_CONTROL_C

#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "VU_Control.h"
#include "MSGEQ7_Setup.h"
#include "LED_Control.h"
#include "LED_Graphics.h"
#include "Grid_Setup.h"
#include <stdio.h>

extern volatile RGB COLOR[11];

extern volatile T16_FLAG FLAG1;

extern volatile UINT32 count32;
extern volatile UINT32 grid_row[12];

/*******************************************************************************
* Function: Pods_VU_Mode1(UINT16 *signal)                                                                     
*                                                                               
* Variables:                                                                    
* *signal -> Points to the adjusted ADC readings for each frequency channel                                                                          
*                                                                               
* Description:                                                                  
* This function displays a customized VU meter animation on the RGB pods.                                                                              
*******************************************************************************/
void Pods_VU_Mode1(UINT16 *signal)
{
	//Check the 60Hz band, if it passes a threshold turn the specified pods on 
	if (signal[0] > 21)
	{
		Fade_Pod(1,COLOR[RED],30);
		Fade_Pod(11,COLOR[RED],30);
		Fade_Pod(21,COLOR[RED],30);
	}	
	
	//Turn the specified pods off	
	else
	{
	 	Fade_Pod(1,COLOR[BLACK],30);
	 	Fade_Pod(11,COLOR[BLACK],30);
	 	Fade_Pod(21,COLOR[BLACK],30);
	 }
	 	
	 	
	//Check the 160Hz band, if it passes a threshold turn the specified pods on 
	if (signal[1] > 22)
	{
		Fade_Pod(2,COLOR[GREEN],30);
		Fade_Pod(12,COLOR[GREEN],30);
	} 	
		
	//Turn the specified pods off	
	else
	{
	 	Fade_Pod(2,COLOR[BLACK],30);
	 	Fade_Pod(12,COLOR[BLACK],30);
	} 	
	 	
	 
	//Check the 400Hz band, if it passes a threshold turn the specified pods on 	
	if (signal[2] > 20)
	{
		Fade_Pod(3,COLOR[PINK],30);
		Fade_Pod(13,COLOR[PINK],30);
	} 	
		
	//Turn the specified pods off	
	else
	{
	 	Fade_Pod(3,COLOR[BLACK],30);
	 	Fade_Pod(13,COLOR[BLACK],30);
	} 	
	 	
	
	//Check the 1kHz band, if it passes a threshold turn the specified pods on  	
	if (signal[3] > 23)
	{
		Fade_Pod(4,COLOR[YELLOW],30);
		Fade_Pod(14,COLOR[YELLOW],30);
	} 	
		
	//Turn the specified pods off	
	else
	{
	 	Fade_Pod(4,COLOR[BLACK],30);
	 	Fade_Pod(14,COLOR[BLACK],30);
	} 	
	 	
	
	//Check the 2.5Hz band, if it passes a threshold turn the specified pods on  	
	if (signal[4] > 23)
	{
		Fade_Pod(5,COLOR[CYAN],30);
		Fade_Pod(15,COLOR[CYAN],30);
	} 	
	
	//Turn the specified pods off		
	else
	{
	 	Fade_Pod(5,COLOR[BLACK],30);
	 	Fade_Pod(15,COLOR[BLACK],30);
	} 	
	 	
	 
	//Check the 6.25kHz band, if it passes a threshold turn the specified pods on 	
	if (signal[5] > 23)
	{
		Fade_Pod(6,COLOR[ORANGE],30);
		Fade_Pod(16,COLOR[ORANGE],30);
	} 	
	
	//Turn the specified pods off		
	else
	{
	 	Fade_Pod(6,COLOR[BLACK],30);
	 	Fade_Pod(16,COLOR[BLACK],30);
	} 	   		
}

/*******************************************************************************
* Function: Pods_VU_Mode1(UINT16 signal)                                                                     
*                                                                               
* Variables:                                                                    
* signal -> Contains the adjusted ADC reading of one of the frequency channels                                                                          
*                                                                               
* Description:                                                                  
* This function displays a customized VU meter animation on the RGB pods.                                                                              
*******************************************************************************/
void Pods_VU_Mode2(UINT16 signal)
{
	RGB meter;

	//Set a new color for the next pod
	meter.red = 200;
	meter.green = 0;
	meter.blue =  0;
  
  //If the signal is higher than the specified threshold, turn the specified pods on
	if (signal > 5)
	{
		Fade_Pod(10,meter,30);
		Fade_Pod(20,meter,30);
	}	
		
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(10,COLOR[BLACK],30);
	 	Fade_Pod(20,COLOR[BLACK],30);
	} 		 

	//Set a new color for the next pod
	meter.red = 400;
	meter.green = 0;
	meter.blue = 0;	
	 	
  //If the signal is higher than the specified threshold, turn the specified pods on
	if (signal > 8)
	{
		Fade_Pod(9,meter,30);
		Fade_Pod(19,meter,30);
	}	
		
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(9,COLOR[BLACK],30);
	 	Fade_Pod(19,COLOR[BLACK],30);
	} 		 	

	//Set a new color for the next pod
	meter.red = 700;
	meter.green = 0;
	meter.blue = 0;
	 	
  //If the signal is higher than the specified threshold, turn the specified pods on
	if (signal > 11)
	{
		Fade_Pod(8,meter,30);
		Fade_Pod(18,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(8,COLOR[BLACK],30);
	 	Fade_Pod(18,COLOR[BLACK],30);
	} 		 	

	//Set a new color for the next pod
	meter.red = 1000;
	meter.green =0 ;
	meter.blue = 0;
	 
  //If the signal is higher than the specified threshold, turn the specified pods on	
	if (signal > 14)
	{
		Fade_Pod(7,meter,30);
		Fade_Pod(17,meter,30);
	}
		
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(7,COLOR[BLACK],30);
	 	Fade_Pod(17,COLOR[BLACK],30);
	} 	

	//Set a new color for the next pod
	meter.red = 2000;
	meter.green = 0;
	meter.blue = 0;
	 
  //If the signal is higher than the specified threshold, turn the specified pods on	
	if (signal > 17)
	{
		Fade_Pod(4,meter,30);
		Fade_Pod(14,meter,30);
	}
	
	//Otherwise turn the specified pods off		
	else
	{
	 	Fade_Pod(4,COLOR[BLACK],30);
	 	Fade_Pod(14,COLOR[BLACK],30);
	} 	

	//Set a new color for the next pod
	meter.red = 6000;
	meter.green = 0;
	meter.blue = 0;
	 	
  //If the signal is higher than the specified threshold, turn the specified pods on
	if (signal > 20)
	{
		Fade_Pod(5,meter,30);
		Fade_Pod(15,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(5,COLOR[BLACK],30);
	 	Fade_Pod(15,COLOR[BLACK],30);
	} 		 	

	//Set a new color for the next pod
	meter.red = 12000;
	meter.green = 0;
	meter.blue = 0;	
	 
  //If the signal is higher than the specified threshold, turn the specified pods on	
	if (signal > 23)
	{
		Fade_Pod(6,meter,30);
		Fade_Pod(16,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(6,COLOR[BLACK],30);
	 	Fade_Pod(16,COLOR[BLACK],30);
	} 		 	

	//Set a new color for the next pod
	meter.red = 30000;
	meter.green = 0;
	meter.blue = 0;	
	 	
  //If the signal is higher than the specified threshold, turn the specified pods on
	if (signal > 25)
	{
		Fade_Pod(3,meter,30);
		Fade_Pod(13,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(3,COLOR[BLACK],30);
	 	Fade_Pod(13,COLOR[BLACK],30);
	} 	 	

	//Set a new color for the next pod
	meter.red = 45000;
	meter.green = 0;
	meter.blue = 0;	
	 
  //If the signal is higher than the specified threshold, turn the specified pods on	
	if (signal > 28)
	{
		Fade_Pod(2,meter,30);
		Fade_Pod(12,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(2,COLOR[BLACK],30);
	 	Fade_Pod(12,COLOR[BLACK],30);
	} 		 	

	//Set a new color for the next pod
	meter.red = 65000;
	meter.green =0;
	meter.blue = 0;	
	 
  //If the signal is higher than the specified threshold, turn the specified pods on	
	if (signal > 30)
	{
		Fade_Pod(1,meter,30);
		Fade_Pod(11,meter,30);
		Fade_Pod(21,meter,30);
	}	
	
	//Otherwise turn the specified pods off	
	else
	{
	 	Fade_Pod(1,COLOR[BLACK],30);
	 	Fade_Pod(11,COLOR[BLACK],30);
	 	Fade_Pod(21,COLOR[BLACK],30);
	} 	 			   	  
}


/*******************************************************************************
* Function: Display_Data(UINT16 *channel)                                                                     
*                                                                               
* Variables:                                                                    
* *channel -> Points to the 7 ADC frequency readings                                                                           
*                                                                               
* Description:                                                                  
* This function can be used for debugging to print the last VU meter signal to
* a terminal program on a PC or phone.                                                                              
*******************************************************************************/
void Display_Data(UINT16 *channel)
{
  UINT8 i;
  
  //Display all 7 frequency band readings
  for (i = 0;i < 7;i++)
    printf("Chan[%d]: %d  \t",i,channel[i]);          
  
  //Add more spacing  
  printf("\r\n");  
}  

/*******************************************************************************
* Function: Bargraph_Update(UINT16 signal)                                                                     
*                                                                               
* Variables:                                                                 
* signal -> Contains the adjusted ADC reading of one of the frequency channels       
*                                                                               
* Description:                                                                  
* This function will display a bargraph VU meter display across the LED grid.                                                                              
*******************************************************************************/
void Bargraph_Update(UINT16 signal)
{
  UINT8 i;
  UINT32 value;
  static UINT8 last_level = 0;
  static UINT32 tmark = 0;
  
  //If the signal is smaller than the previous one, only decrease the bargraph by 1
  if (signal < last_level)
	{
		//Create a time delay so that the bargraph isnt sporadic
	  if (Time_Check(&tmark,11))
	  {
		  //Decrease the bargraph by 1 level
			signal = last_level - 1;
			
			//Modify the grid to display the new bargraph size
			for (i = 0;i < GRID_Y_MAX;i++)
			{
				value = ((UINT32)1 << signal) - 1;
				value <<= 1;
				value |= 1;
				
				grid_row[i] = value;
			}	
			
			//Store the current reading for the next loop
		  last_level = signal;			
	 }
  }
 
  //The signal is greater than the last one 
  else 
  {
	 
	 //Update the grid to display the new bargraph size
	 for (i = 0;i < GRID_Y_MAX;i++)
	 {
	 	 value = ((UINT32)1 << signal) - 1;
	 	 value <<= 1;
		 value |= 1;
				
		 grid_row[i] = value;
	 }		 		  
	}		
	
	//Update the LED grid
	UPDATE_FRAME();	 
}

/*******************************************************************************
* Function: Grid_VU_Mode1(UINT16 signal)                                                                     
*                                                                               
* Variables:       
* signal -> Contains the adjusted ADC reading of one of the frequency channels                                                              
*                                                                               
* Description:         
* This function displays a customized VU meter animation on the LED grid.                                                                
*******************************************************************************/
void Grid_VU_Mode1(UINT16 signal)
{
  static UINT8 last_level = 0;
  static UINT32 tmark = 0;
  
  //Clear the grid data
  Clear_Grid();
  
  //Adjust the signal
  signal = (float)signal * 0.6;
  
  //If the signal is smaller than the previous one, only decrease the level by 1
  if (signal < last_level)
	{
		//If the signal is a 0 or a 1, reset it to 0
  	if (last_level == 1 || last_level == 0)
  	{
    	signal = 0;
    	return;
    } 	
  		
		//Create a time delay so that the animation isnt sporadic
	  if (Time_Check(&tmark,8))
	  {
		  //Decrease the bargraph by 1 level
			signal = last_level - 1;
			
			//Draw a circle in the middle of the grid, with its radius determined by the
			//intensity of the signal
			Draw_Circle(15,5,signal);
			
			//Store the current reading for the next loop
		  last_level = signal;			
	  }
  }
 
  //The new signal reading is larger than the last, update the animation
  else 
	  Draw_Circle(15,5,signal);			  	
	
	//Update the LED grid
  UPDATE_FRAME(); 
}

/*******************************************************************************
* Function: Grid_VU_Mode2(UINT16 *signal)                                                                     
*                                                                               
* Variables:   
* *signal -> Points to the adjusted ADC readings for each frequency channel                                                                     
*                                                                               
* Description:                        
* This function displays a customized VU meter animation on the LED grid.                                            
*******************************************************************************/
void Grid_VU_Mode2(UINT16 *signal)
{
  UINT8 i;
  static UINT8 last_level[7] = {0,0,0,0,0,0,0};
  UINT8 div = 14;
  
  //Clear the grid data
  Clear_Grid();
  
  //Adjust the signal levels as each circle has a smaller maximum radius
  for (i = 0;i < 7;i++)
  	signal[i] /= 5;
  
  //If the signal is smaller than the previous one, only decrease the level by 1
  if (signal[0] < last_level[0])
 	{
		//Offset the signal
		signal[0] = last_level[0] - div;
		
		//Draw a small circle in a specific location on the LED grid, with the 
		//radius being determined by the instensity of the signal	
		Draw_Circle(4,3,signal[0]);
			
		//Store the current reading for the next loop
		last_level[0] = signal[0];			
	
  }
 
  //The new signal reading is larger than the last, update the animation
  else 
	  Draw_Circle(4,3,signal[0]);		
  
  //If the signal is smaller than the previous one, only decrease the level by 1
  if (signal[4] < last_level[4])
	{	
		//Offset the signal
		signal[4] = last_level[4] - div;
			
		//Draw a small circle in a specific location on the LED grid, with the 
		//radius being determined by the instensity of the signal	
		Draw_Circle(12,7,signal[4]);
			
		//Store the current reading for the next loop
		last_level[4] = signal[4];			
	
  }
 
  //The new signal reading is larger than the last, update the animation
  else 
	  Draw_Circle(12,8,signal[4]);		
  
  //If the signal is smaller than the previous one, only decrease the level by 1
  if (signal[1] < last_level[1])
	{
		//Offset the signal
		signal[1] = last_level[1] - div;
			
		//Draw a small circle in a specific location on the LED grid, with the 
		//radius being determined by the instensity of the signal	
		Draw_Circle(27,8,signal[1]);
			
		//Store the current reading for the next loop
		last_level[1] = signal[1];			
	
  }
 
  //The new signal reading is larger than the last, update the animation
  else 
	  Draw_Circle(27,8,signal[1]);		
  
  //If the signal is smaller than the previous one, only decrease the level by 1
  if (signal[5] < last_level[5])
	{	  
		//Offset the signal
		signal[5] = last_level[5] - div;
			
		//Draw a small circle in a specific location on the LED grid, with the 
		//radius being determined by the instensity of the signal	
		Draw_Circle(20,3,signal[5]);
			
		//Store the current reading for the next loop
		last_level[5] = signal[5];			
  }
 
  //The new signal reading is larger than the last, update the animation
  else 
	  Draw_Circle(20,3,signal[5]);			  	
		
	//Update the LED grid
	UPDATE_FRAME();	 
}
 
#endif