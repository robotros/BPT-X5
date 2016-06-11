/*******************************************************************************
* Title: LED_Control.c                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions which are used to control the RGB pods, LEDx
* channels, ballwashers, etc.                                                                                                                           
*******************************************************************************/
   
#ifndef LED_CONTROL_C
#define LED_CONTROL_C   
   
#include "Main_Includes.h"
#include "Delay_Setup.h"
#include "LED_Control.h"
#include "TLC5955_Setup.h"
#include "Grid_Setup.h"
#include "FAT32_Setup.h"

/*************************************************
*               Global Variables                 *
*************************************************/
extern volatile UINT8 seq[SEQ_AMOUNT];

extern UINT16 TLC_data[96];
extern volatile T16_FLAG FLAG1;
extern volatile UINT16 RINGn[16];
extern volatile UINT16 ring_update;
extern volatile UINT16 ring_brightness;

extern volatile UINT32 pod_update;
extern volatile UINT32 grid_row[12];

extern volatile RGB PODn[21];
extern volatile  RGB COLOR[11];
extern volatile const RGB DEFAULT_COLOR[11];

extern volatile RGB_FADE RGB_DIFF[21];
extern volatile RING_FADE RING_DIFF[16];

/*******************************************************************************
* Function: Update_Channel(UINT16 channel, UINT16 duty_cycle)                                                                   
*                                                                              
* Variables:                                                                   
* channel -> The TLC5955 channel that you want to modify
* duty_cycle -> The duty cycle that you want to set the channel at                                                                         
*                                                                              
* Description:                                                                 
* This function allows one to modify the duty cycle of a specified channel on
* a TLC5955 IC. There are 2 channels on the X5 board, giving us a total of 96
* channels (0 - 95).                                                                              
*******************************************************************************/
void Update_Channel(UINT16 channel, UINT16 duty_cycle)
{
	//Set the specified channel to the specified duty cycle
  TLC_data[channel] = duty_cycle;
  
  //Update the channel
  TLC5955_Update();
}  

/*******************************************************************************
* Function: RGB_Pod(UINT8 pod, UINT16 red, UINT16 green, UINT16 blue)                                                                    
*                                                                              
* Variables:                                                                   
* red -> The 16-bit PWM value for the RGB pods red channel
* green -> The 16-bit PWM value for the RGB pods green channel
* blue -> The 16-bit PWM value for the RGB pods blue channel                                                                           
*                                                                              
* Description:                                                                 
*                                                                              
*******************************************************************************/
void RGB_Pod(UINT8 pod, UINT16 red, UINT16 green, UINT16 blue)
{
  UINT16 *index;
  UINT8 loc;
  
  //Point to the location of the RGB pod data  
  index = &TLC_data[0];
  
  //If the specified pod is #21, that is reserved for the RGB underlighting; Adjust
  //the channel location accordingly
  if (pod == 21)
  	loc = 93;
  
  //It is one of the twenty pods, calculate the location of that pods data in the array
  else
  	loc = (pod - 1) * 3;
  	
  //Update the pods channels
  PODn[pod-1].red = red;
  PODn[pod-1].green = green;
  PODn[pod-1].blue = blue;
  
  //Point to the start location of the specific pod in the RGB pod data array
  index += loc;
  
  //Update the new r,g,b channel values for the pod
  *index = red;
  *(++index) = green;
  *(++index) = blue; 
  
  //Update the channels
  TLC5955_Update();
}  

/*******************************************************************************
* Function: RGB_Underlighting(RGB underlight)                                                                
*                                                                              
* Variables:                                                                   
* underlight -> An RGB struct which contains the color of the underlighting                                                                          
*                                                                              
* Description:                                                                 
* This function will change the color of the RGB underlighting.                                                                        
*******************************************************************************/
void RGB_Underlighting(RGB underlight)
{
	UINT16 *index;
  
  //Update the 21st pod data which pertains to the RGB underlighting
  PODn[20].red = underlight.red;
  PODn[20].green = underlight.green;
  PODn[20].blue = underlight.blue;
  
  //Set the start location of the RGB underlighting data in the RGB pod array
  index = &TLC_data[0] + 93;
  
  //Update the underlighting to the new color
  *index = underlight.red;
  *(++index) = underlight.green;
  *(++index) = underlight.blue; 
  
  //Update the channels
  TLC5955_Update();
}	 

/*******************************************************************************
* Function: Adjust_Pod_Brightness(UINT8 *brightness)                                                                
*                                                                              
* Variables:                                                                   
* brightness -> The brightness value of the RGB pods                                                                          
*                                                                              
* Description:                                                                 
* This function will adjust the brightness of the RGB pods on a 1 - 20 scale.                                                                        
*******************************************************************************/
void Adjust_Pod_Brightness(UINT8 *brightness)
{
  UINT8 i;
  UINT8 modifier;
  
  //Do not overrun the max brightness of the pods
  if (*brightness > POD_BRIGHTNESS_MAX)
    *brightness = POD_BRIGHTNESS_MAX;
  
  //Do not overrun the minimum brightness of the pods
  else if (*brightness < POD_BRIGHTNESS_MIN)
    *brightness = POD_BRIGHTNESS_MIN;
  
  //Calculate the brightness of the pod
  modifier = (POD_BRIGHTNESS_MAX + 1) - *brightness;
  
  //Adjust each COLOR value to reflect the new brightness value
  for (i = 0;i < COLOR_COUNT;i++)
  {
    COLOR[i].red = DEFAULT_COLOR[i].red / modifier;
    COLOR[i].green = DEFAULT_COLOR[i].green / modifier;
    COLOR[i].blue = DEFAULT_COLOR[i].blue / modifier;
    
    //Keep the minimum brightness at a specified level unless it is completely off
    if (COLOR[i].red < 500 && DEFAULT_COLOR[i].red != 0)
      COLOR[i].red = 500;
    
    //Keep the minimum brightness at a specified level unless it is completely off
    if (COLOR[i].green < 500 && DEFAULT_COLOR[i].green != 0)
      COLOR[i].green = 500;
    
    //Keep the minimum brightness at a specified level unless it is completely off
    if (COLOR[i].blue < 500 && DEFAULT_COLOR[i].blue != 0)
      COLOR[i].blue = 500;
  }  
}
	
/*******************************************************************************
* Function: Fill_Grid(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will set all of the LED bits in the grid. In order to write these
* new values to the LED grid, we must call UPDATE_FRAME() after this call.                                                                        
*******************************************************************************/         
void Fill_Grid(void)
{
 UINT8 i;
 
 //Set all bits (LEDs)
 for (i = 0;i < GRID_Y_MAX;i++)
   grid_row[i] = 0xFFFFFFFF;
}

/*******************************************************************************
* Function: Clear_Grid(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will clear all of the LED bits in the grid. In order to write these
* new values to the LED grid, we must call UPDATE_FRAME() after this call.     
*******************************************************************************/         
void Clear_Grid(void)
{
 UINT8 i;
 
 //Clear all bits (LEDs)
 for (i = 0;i < GRID_Y_MAX;i++)
   grid_row[i] = 0x00000000;
} 


/*******************************************************************************
* Function: Disable_All_Features(void)                                                                
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function is just a quick way to turn off all of the features on the table.                                                                        
*******************************************************************************/
void Disable_All_Features(void)
{  
	//Fade all features off
  Fade_All_Pods(COLOR[BLACK],200);
  Fade_Pod(21,COLOR[BLACK],200);
  Fade_All_Rings(0,200,ALL_LED_RINGS);
  
  //Clear the LED grid
  Clear_Grid();
  UPDATE_FRAME();
  
  //Allow enough time for features to fade off
  Delay_ms(400);
  
  //Ensure that all features are off by forcing them off
  Set_All_Pods(COLOR[BLACK]);
  Set_All_Rings(0,ALL_LED_RINGS);
  Pod_Set_Color(21,COLOR[BLACK]);
}  

/*******************************************************************************
* Function: Update_Ring(UINT8 ring, UINT16 duty_cycle)                                                                    
*                                                                              
* Variables:                                                                   
* ring -> The LED ring that is to be modified
* duty_cycle -> The PWM duty of the specified ring                                                                          
*                                                                              
* Description:                                                                 
* This function will update any of the 16 LEDx channels with a specified PWM value.
* The 'ring' variable will accept any value from 1 - 16, each ring is respective
* to its LEDx designation, with x being the ring number.                                                                             
*******************************************************************************/
void Update_Ring(UINT8 ring, UINT16 duty_cycle)
{
  //If the selection made is out of bounds, return the function
  if ((ring < 1) || (ring > 16))
  	return;
  
  //Update the new LED ring data
  RINGn[ring-1] = duty_cycle;
  
  //Save modified data in appropriate TLC5955 register
  TLC_data[(LED1 - 1) + ring] = duty_cycle; 
  
  //Call for an update
  TLC5955_Update();
}  

/*******************************************************************************
* Function: Fade_All_Pods(RGB NEW, UINT16 fade_rate)                                                               
*                                                                              
* Variables:                                                                                                                                         
* NEW -> An RGB struct which holds the color that the pod is fading to                                                                            
* fade_rate -> The amount of delay between each fade step                                                                          
*                                                                              
* Description:                                                                 
* This function will fade all of the RGB pods to one specific color over a
* specified fade rate.                                                                       
*******************************************************************************/
void Fade_All_Pods(RGB NEW, UINT16 fade_rate)                   
{
  UINT8 i;
  
  //Set each pod to fade to the specified color
  for (i = 0;i < 20;i++)
    Fade_Pod(i+1,NEW,fade_rate);
}
	
/*******************************************************************************
* Function: Fade_All_Rings(UINT16 duty_cycle, UINT16 fade_rate, UINT8 mode)                                                                
*                                                                              
* Variables:                                                                   
* duty_cycle -> The 16-bit PWM duty cycle (0 - 65535)
* fade_rate -> Determines the amount of time it takes to complete the fade
* mode -> Determines whether it will adjust the ball washer LED rings                                                                          
*                                                                              
* Description:                                                                 
* This function will adjust all of the LED rings PWM duty cycle to the value of
* 'duty_cycle'. If mode is equal to zero, this function will not modify the ball
* washer LED rings and if mode is a non-zero value it will adjust the ball washer
* rings unless the BW_ACTIVE flag is set. If the variable 'mode' is set to 0, 
* the ball washer LED rings will not be modified. If it is set to 1, all LED rings
* on the table will be modified unless a ball washer is in use.                                                                       
*******************************************************************************/
void Fade_All_Rings(UINT16 duty_cycle, UINT16 fade_rate, UINT8 mode)                   
{
  UINT8 i;
  UINT8 amount;
  
  //If mode is set to a non-zero value and BW_ACTIVE is cleared, we will adjust
  //all of the LED rings
  if ((mode != 0) && (BW_ACTIVE == 0))
    amount = 12;
 
  //Otherwise adjust the 8x main LED rings on the rails   
  else
    amount = 8;
  
  //Update the LED rings  
  for (i = 0;i < amount;i++)
    Fade_Ring(i+1,duty_cycle,fade_rate);
}

/*******************************************************************************
* Function: Fade_Pod(uint8 pod, RGB NEW, uint16 delay)                                                                    
*                                                                              
* Variables:                                                                   
* pod -> The pod that is to be modified 
* NEW -> The new RGB color that the pod will fade to
* delay -> The amount of time it takes to complete the fade                                                                         
*                                                                              
* Description:                                                                 
* This function will fade one of the twenty pods (1 - 20) or the RGB underlighting
* (21) from its previous color to another color which is designated by the 'NEW'
* RGB struct. The amount of time that it takes the pod to fade depends on the 
* value of delay, with each increment adding approximately 1ms to the fade time.                                                                             
*******************************************************************************/
void Fade_Pod(uint8 pod, RGB NEW, uint16 delay)
{ 
	//If the specified pod does not exist, exit function
	if ((pod == 0) || (pod > 21))
		return;
	
  //Calculate the fade step value for RED, GREEN and BLUE values
  RGB_DIFF[pod-1].red = (float)(NEW.red - PODn[pod-1].red) / delay;
  RGB_DIFF[pod-1].green = (float)(NEW.green - PODn[pod-1].green) / delay;
  RGB_DIFF[pod-1].blue = (float)(NEW.blue - PODn[pod-1].blue) / delay;
  RGB_DIFF[pod-1].delay = delay; 
  
  //Set the fade from RGB pod color
  RGB_Pod(pod,PODn[pod-1].red,PODn[pod-1].green,PODn[pod-1].blue); 
  
  //Set the flag bit that corresponds the pod # that is to be updated 
  pod_update |= ((UINT32)1 << (UINT32)((UINT32)pod-1));
}

/*******************************************************************************
* Function: Fade_State(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function keeps track of the state of the RGB pods and the LED rings.                                                                              
* The TIMER3 interrupt calls this function to see if any pods or LED rings need                                                                             
* to be updated from a fade routine.                                                                             
*******************************************************************************/
void Fade_State(void)
{
  UINT8 i;

  //If no flags are set for either the pods or the rings, return the function
  if (pod_update == 0 && ring_update == 0)
    return;

  //Loop through specified pods, if a flag is set for that pod, update its values
	for (i = 0;i < 21;i++)
	{
		//Cycle through all of the pods
	  if ((pod_update >> i) & 0x01)
	  {
  	  //Adjust the pod one fade step
	    PODn[i].red += RGB_DIFF[i].red;
	    PODn[i].green += RGB_DIFF[i].green;
	    PODn[i].blue += RGB_DIFF[i].blue;   
	    RGB_DIFF[i].delay--;
	      
	    //Update the pod with the new values
	    RGB_Pod(i+1,PODn[i].red,PODn[i].green,PODn[i].blue);
	      
	    //If the delay is over, clear the pods update flag
	    if (!RGB_DIFF[i].delay)
	       pod_update &= (~((UINT32)1 << i)); 
	  }
	} 
	
  //Check LED rings. If a flag is set, update the values
	for (i = 0;i < 16;i++)
	{
		//Cycle through all of the LEDx channels
	  if ((ring_update >> i) & 0x01)
	  {
  	  //Adjust the ring one fade step
		  RINGn[i] += RING_DIFF[i].value; 
	    RING_DIFF[i].delay--;
	    
	    //Update the LED ring
	    Update_Ring(i+1, RINGn[i]);
	    
	    //If the delay is over, clear the rings update flag
	    if (!RING_DIFF[i].delay)
	       ring_update &= (~(1 << i)); 
	  }
	}   
}
/*******************************************************************************
* Function: Fade_Ring(UINT8 LED, float new, UINT16 delay)                                                                      
*                                                                               
* Variables:                                                                    
* LED -> The LED ring that is to be modified                                                                     
* new -> The new PWM duty cycle that the ring will be set to
* delay -> The amount of time that it takes to fade to the new PWM value
*                                                                               
* Description:                                                                  
* This function allows a user to fade an LED ring (1 - 16) to a specified PWM
* value that is denoted by the variable 'new'. The amount of time that it takes
* to fade is dependant on the 'delay' variable, with each increment adding 
* approximately 1ms more to the fade time.                                                                              
*******************************************************************************/
void Fade_Ring(UINT8 LED, float new, UINT16 delay)
{
	//If the specified LED ring does not exist, return the function
	if ((LED == 0) || (LED > 16))
		return;
	
  //Calculate the fade step value
  RING_DIFF[LED-1].value = (float)((float)new - (float)RINGn[LED-1]) / delay;
  
  //Set the brightness of the LED ring that it is fading from
  RING_DIFF[LED-1].delay = delay; 
  
  //Set the flag bit that corresponds the LED ring that is to be updated 
  ring_update |= ((UINT16)1 << (UINT16)(LED-1)); 
}  

/*******************************************************************************
* Function: Pod_Set_Color(UINT8 pod, RGB color)                                                                    
*                                                                              
* Variables:                                                                   
* pod -> The pod that is to be modified
* color -> An RGB struct that stores the color of the pod                                                                         
*                                                                              
* Description:                                                                 
* This function simply allows the user to update a pod with an RGB structure instead
* of putting the red, green and blue values in manually.                                                                             
*******************************************************************************/
void Pod_Set_Color(UINT8 pod, RGB pod_color)
{
  //Modify the RGB colors of the pod to achieve the color specified
  RGB_Pod(pod,pod_color.red,pod_color.green,pod_color.blue);
}  	
	
/*******************************************************************************
* Function: Set_All_Pods(RGB color)                                                             
*                                                                              
* Variables:                                                                   
* color -> An RGB struct color value                                                                       
*                                                                              
* Description:                                                                 
* This function will set all twenty of the RGB pods to the same color.                                                                     
*******************************************************************************/
void Set_All_Pods(RGB color)
{
  UINT8 i;
  
  //Set all of the pods to the specified color
  for (i = 0;i < 20;i++)
  	Pod_Set_Color(i+1,color);   
}	
	
/*******************************************************************************
* Function: Set_All_Rings(UINT16 duty_cycle, UINT8 mode)                                                           
*                                                                              
* Variables:                                                                   
* duty_cycle -> The 16-bit PWM duty cycle (0 - 65535)
* mode -> Determines whether it will adjust the ball washer LED rings                                                                          
*                                                                              
* Description:                                                                 
* This function will adjust all of the LED rings PWM duty cycle to the value of
* 'duty_cycle'. If mode is equal to zero, this function will not modify the ball
* washer LED rings and if mode is a non-zero value it will adjust the ball washer
* rings unless the BW_ACTIVE flag is set.                                                                         
*******************************************************************************/
void Set_All_Rings(UINT16 duty_cycle, UINT8 mode)
{
  UINT8 i;
  UINT8 amount;
  
  //If mode is set to a non-zero value and BW_ACTIVE is cleared, we will adjust
  //all of the LED rings
  if ((mode != 0) && (BW_ACTIVE == 0))
    amount = 16;
 
  //Only adjust the 8x main LED rings on the rails   
  else
    amount = 8;
  
  //Update the LED rings  
  for (i = 1;i <= amount;i++)
    Update_Ring(i,duty_cycle);
}  

#endif
