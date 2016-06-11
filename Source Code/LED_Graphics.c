/*******************************************************************************
* Title: LED_Graphics.c                                                                       
* Version: 1.0                                                                   
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file contains various functions that are used to display animations on the RGB pods,
* LED grid and LED rings. 
*
* In the diagram below each 'x' represents one LED. This is how the data is packed and
* arranged for the LED grid. The LED grid shown below is 32x12 LEDs in size but can be 
* .
*                              
*                             32-Bit Data Array 'LED_data[12]'                      
*           0 1 2 3 4 5 6 7 8 9 . . . . . . . . . . . . . . . . . . . . . . 31
*           | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |                                          
*       0 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                   
*       1 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                       
*       2 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                   
*       3 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                
*    B  4 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                                         
*    i  5 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                         
*    t  6 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                        
*    s  7 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                      
*       8 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                      
*       9 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                                      
*      10 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                          
*      11 - x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x                                      
*                                                                                                  
*******************************************************************************/

#ifndef LED_GRAPHICS_C
#define LED_GRAPHICS_C

#include "Main_Includes.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Font_5x7.h"
#include "TLC5955_Setup.h"
#include "LED_Graphics.h"
#include "LED_Control.h"
#include "Grid_Setup.h"
#include "VU_Control.h"
#include "Delay_Setup.h"

/*************************************************
*               Global Variables                 *
*************************************************/
extern volatile char global_str[64];

extern volatile UINT8 str_index;
extern volatile UINT8 str_length;
extern volatile UINT8 DA_grid[4];
extern volatile UINT8 DA_pods[4];
extern volatile UINT8 DA_rings[4];
extern volatile UINT8 frame_update;
extern volatile UINT8 seq[SEQ_AMOUNT];


extern volatile UINT8 VU_Meter;

extern UINT16 VU_signal[7];
extern volatile T16_FLAG FLAG1;
extern volatile UINT8 error_code;
extern volatile UINT16 TLC_data[96];
extern volatile UINT16 ring_brightness;

extern volatile UINT32 IR_sensors;
extern volatile UINT32 count32;
extern volatile UINT32 grid_row[12];
extern volatile UINT32 grid_frame[12];

extern volatile RGB PODn[21];
extern volatile RGB COLOR[11];
extern volatile RGB CUSTOM_COLOR1;
extern volatile RGB CUSTOM_COLOR2;
	
	
/*******************************************************************************
* Function:                                                                  
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
*                                                                             
*******************************************************************************/	
UINT8 Intro_Animation(void)
{
  static UINT16 delay = 300;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[23] == 0xFF)
  {
    seq[23] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[23] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[23];
    tmark = count32;  
  }
    
  //Cycle through the various frames; Each with a interrupt driven delay 
  switch (seq[23])
  { 
    case 0:
    				//Marvin Animation
    				grid_row[0]   = 0x00183000;
						grid_row[1]   = 0x00145000;
						grid_row[2]   = 0x00139000;
						grid_row[3]   = 0x00101000;
						grid_row[4]   = 0x00082000;
						grid_row[5]   = 0x00044000;
						grid_row[6]   = 0x00044000;
						grid_row[7]   = 0x00082000;
						grid_row[8]   = 0x00101000;
						grid_row[9]   = 0x00139000;
						grid_row[10]  = 0x00145000;
						grid_row[11]  = 0x00183000;
						UPDATE_FRAME();
						
						delay = TIME_DELAY_3S;
    	 			break;
    	 			
    //Here we set the delay to a very large value as we don't want the
    //interrupt delay check to move on to the next animation until we 
    //know that all of the text is off of the grid.        
    case 1: 
    				Set_Scrolling_Text("HAPPY BIRTHDAY 3D HUBS!");
    				
    				//If text has finished scrolling, reset scroll flag and set
    				//delay equal to 1 so that the Time_Check(a,b) below advances to 
    				//the next animation	
    				if (SCROLL_FINISHED)
    				{
    				 delay = 1;
    				 SCROLL_FINISHED = 0;
    				} 
    				
    				//If the scroll has not finished yet, set delay to a high value
    				//so that Time_Check(a,b) does not continue to the next animation
    		    //until the scrolling has finished
    			  else
            delay = TIME_DELAY_1M;  
            
            break;
     
     //Display the 3DHubs Logo       
    case 2: 
     				grid_row[0]   = 0x00000000;
						grid_row[1]   = 0x00033000;
						grid_row[2]   = 0x0007F800;
						grid_row[3]   = 0x001FFE00;
						grid_row[4]   = 0x001FFE00;
						grid_row[5]   = 0x001FFE00;
						grid_row[6]   = 0x001FFE00;
						grid_row[7]   = 0x001FFE00;
						grid_row[8]   = 0x000FFC00;
						grid_row[9]   = 0x0003F000;
						grid_row[10] = 0x0000C000;
						grid_row[11] = 0x00000000;
						UPDATE_FRAME();

						delay = TIME_DELAY_3S;
						break;
    			
  }  
     
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[23]++;
  
  //If all sequences have been performed, reset all sequences
  //and return a 0 to indicate that the routine is finished
  if (seq[23] > 2)
  {
	  VU_Meter = 0;
    seq[23]= 0;
    return 0; 
  }  
  
  //The function has not finished
  return 1;
}	
	
	/*******************************************************************************
* Function: Animate_On_Detection(void)                                                             
*                                                                              
* Variables:                                                                   
* N/A                                                                       
*                                                                              
* Description:                                                                 
* This function will run one animation on the LED grid continuously until a cup
* gets removed from the table. Once a pod is removed, it will run a new animation,
* in this case End_Blast(side), to coincide with the cup being removed. Once a
* cup is removed, the current animation will be halted and End_Blast(side) will                                                                     
* be called which will shoot a line across the LED grid to whichever side that the
* cup was removed from, it will then do a small 'exploding' effect when it 'hits'
* the other side. Once the animation finishes, the previous animation will pick up
* where it left off.
*
* In this case, the 'detected pod removal' animation that I chose to use is
* End_Blast(side) and the original animation is Pong_Game().
*******************************************************************************/
void Animate_On_Detection(void)
{
  INT8 pod = 0;
  static UINT8 tracker = 0;
  
  //If a previous 'cup removal detected' animation is not running check the 
  //pod detection states
  if (tracker == 0)
  {
    //If a pods detection state has been modified it will be saved in 'pod'
    //Otherwise 'pod' will equal 0, indicating no change
    pod = On_Pod_Change(IR_sensors);
    
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 1 & 10, the removed cup was
    //on the MASTER SIDE. Start the 'detected pod removal' animation.
    if ((pod < 0) && (abs(pod) < 11))
      tracker = End_Blast(MASTER_SIDE);
    
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 11 & 20, the removed cup was
    //on the SECONDARY SIDE. Start the 'detected pod removal' animation.
       else if ((pod < 0) && (abs(pod) < 21))
      tracker = End_Blast(SECONDARY_SIDE);
    
    //There was no change in the pods detection states, continue original animation  
    else
    	Pong_Animation();
  }
  
  //A 'detected cup removal' animation has not finished yet. Allow it to finish
  else
    tracker = End_Blast(tracker);     
}  
	
	/*******************************************************************************
* Function: Animate_On_Detection(void)                                                             
*                                                                              
* Variables:                                                                   
* N/A                                                                       
*                                                                              
* Description:                                                                 
* This function will run one animation on the LED grid continuously until a cup
* gets removed from the table. Once a pod is removed, it will run a new animation,
* in this case End_Blast(side), to coincide with the cup being removed. Once a
* cup is removed, the current animation will be halted and End_Blast(side) will                                                                     
* be called which will shoot a line across the LED grid to whichever side that the
* cup was removed from, it will then do a small 'exploding' effect when it 'hits'
* the other side. Once the animation finishes, the previous animation will pick up
* where it left off.
*
* In this case, the 'detected pod removal' animation that I chose to use is
* End_Blast(side) and the original animation is Pong_Game().
*******************************************************************************/
void Corner_Circles_On_Detection(void)
{
  INT8 pod = 0;
  static UINT8 tracker = 0;
  
  //If a previous 'cup removal detected' animation is not running check the 
  //pod detection states
  if (tracker == 0)
  {
    //If a pods detection state has been modified it will be saved in 'pod'
    //Otherwise 'pod' will equal 0, indicating no change
    pod = On_Pod_Change(IR_sensors);
    
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 1 & 10, the removed cup was
    //on the MASTER SIDE. Start the 'detected pod removal' animation.
    if ((pod < 0) && (abs(pod) < 21))
      tracker = Corner_Circles();
    
    //There was no change in the pods detection states, continue original animation  
    else
    	Pong_Animation();
  }
  
  //A 'detected cup removal' animation has not finished yet. Allow it to finish
  else
    tracker = Corner_Circles();     
} 
	
	/*******************************************************************************
* Function: Animate_On_Detection(void)                                                             
*                                                                              
* Variables:                                                                   
* N/A                                                                       
*                                                                              
* Description:                                                                 
* This function will run one animation on the LED grid continuously until a cup
* gets removed from the table. Once a pod is removed, it will run a new animation,
* in this case End_Blast(side), to coincide with the cup being removed. Once a
* cup is removed, the current animation will be halted and End_Blast(side) will                                                                     
* be called which will shoot a line across the LED grid to whichever side that the
* cup was removed from, it will then do a small 'exploding' effect when it 'hits'
* the other side. Once the animation finishes, the previous animation will pick up
* where it left off.
*
* In this case, the 'detected pod removal' animation that I chose to use is
* End_Blast(side) and the original animation is Pong_Game().
*******************************************************************************/
void Scrolling_Arrows_On_Detection(void)
{
  INT8 pod = 0;
  static UINT8 tracker = 0;
  
  //If a previous 'cup removal detected' animation is not running check the 
  //pod detection states
  if (tracker == 0)
  {
    //If a pods detection state has been modified it will be saved in 'pod'
    //Otherwise 'pod' will equal 0, indicating no change
    pod = On_Pod_Change(IR_sensors);
    
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 1 & 10, the removed cup was
    //on the MASTER SIDE. Start the 'detected pod removal' animation.
    if ((pod < 0) && (abs(pod) < 11))
      tracker = Scrolling_Arrows(SCROLL_GRID_LEFT);
    
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 11 & 20, the removed cup was
    //on the SECONDARY SIDE. Start the 'detected pod removal' animation.
       else if ((pod < 0) && (abs(pod) < 21))
      tracker = Scrolling_Arrows(SCROLL_GRID_RIGHT);
    
    //There was no change in the pods detection states, continue original animation  
    else
    	Pong_Animation();
  }
  
  //A 'detected cup removal' animation has not finished yet. Allow it to finish
  else
    tracker = Scrolling_Arrows(tracker);   
}  


/*******************************************************************************
* Function: Ball_Washer(UINT8 bw, UINT16 fan_speed, UINT16 pump_speed)                                                                    
*                                                                              
* Variables:                                                                   
* bw -> The ball washer that is to be updated (#1 or #2)                                                                            
* fan_speed -> The speed of the fan                                                                             
* pump_speed -> The speed of the water pump                                                                       
*                                                                              
* Description:                                                                 
* This function controls the speed of the fan and water pump for both ball washers.
* The fan and pump motors are connected to outputs 12, 13, 14 and 15 on the 5th
* TLC5940 chip, which also controls LED rings #1 - #12. Therefore we can use the
* same functions that control the LED rings to control the ball washers.                                                                             
*******************************************************************************/
void Ball_Washer(UINT8 bw, UINT16 fan_speed, UINT16 pump_speed)
{
  //Update the fan and water pump speeds for Ball Washer #1
	if (bw == 1)
    Update_Channel(BW1_FAN,fan_speed);
	
	//Update the fan and water pump speeds for Ball Washer #2
	else
    Update_Channel(BW2_FAN,fan_speed);
}

//////////////////////////////////////////////////////////////////////////////////////
//This function works, but is being edited. Some of the switch statements get bypassed
/*******************************************************************************
* Function: Ball_Washers_Detect(UINT8 detection)                                                            
*                                                                              
* Variables:     
* detection -> This variable should contain the sensor_detect data from Check_Sensors()                                                               
*                                                                              
* Description:  
* This function will take in an 8-bit variable (detection) which contains the 
* object detection data for the ball washers. The bit layout of the 'detection'
* is below this paragraph. This is to be used in conjunction with the return
* value from the function Check_Sensors(). The bits must be shifted 20 places to
* the left so that we mask off the pod sensor bits and only use the ball washer
* object detection bits. Here is an example:
*
*	sensors = Check_Sensors();
*	Ball_Washers_Detect(sensors>>20);
*
* bit 0 - Ball Washer #1 Entry Sensor
* bit 1 - Ball Washer #1 Exit Sensor
* bit 2 - Ball Washer #2 Entry Sensor
* bit 3 - Ball Washer #2 Exit Sensor
*
*******************************************************************************/
void Ball_Washers_Detect(UINT8 detection)
{    
  static UINT8 tracker = 0;		
  static UINT8 repeats = 0;
  static UINT16 delay = 0;
  static UINT32 tmark = 0;
	
	//This switch statement will keep all of the sequences of the ball washers
	//in the proper order through the variable 'tracker'
	switch (tracker)
	{
  	//Check to see if either ball washer has detected a ball. Only one ball washer can 
  	//be used at a time and Ball Washer #1 takes precedence if a ball is dropped into
  	//each ball washer at the same time. The reason we only use one ball washer at a time
  	//is to ensure that we don't overheat the ULN2803A driver by overloading it.
  	case 0:
  	        //Check to see if Ball Washer #1 has detected a ball
            if (detection & BW1_ENTRY)
            { 
              
          	  //Set this bit so that other functions know that the ball washer is active
          	  BW_ACTIVE = 1;
          	  
          	  //If the ball washer LED rings are fading in an animation, stop them
//          	  ring_update &= 0x0FF;
          	  
          	  // Turn off the ball washer LED rings in case they are already on
          	  Update_Ring(9,0);
          	  Update_Ring(10,0);
          	  Update_Ring(11,0);
          	  Update_Ring(12,0);
          	  
              //Turn on the the entry LED ring and start the water pump
          	  Fade_Ring(9,ring_brightness,120);
          	  Ball_Washer(BW1,BW1_FAN_SPEED,BW1_PUMP_SPEED);
          	  
          	  //Update tracker and set a delay of 1.76ms (220 * 8ms per count32 increment)
          	  //Update tmark as we will be using Time_Check() to provide the delay
          	  tracker = 1;
          	  delay = 4880;
          	  tmark = count32;
            }
	   	      
	   	      //Check to see if Ball Washer #2 has detected a ball
            else if (detection & BW2_ENTRY)
            { 
              
              //Set this bit so that other functions know that the ball washer is active
          	  BW_ACTIVE = 1;
          	  
          	  //If the ball washer LED rings are fading in an animation, stop them
//          	  ring_update &= 0x0FF;
          	 
          	  // Turn off the ball washer LED rings in case they are already on
          	  Update_Ring(9,0);
          	  Update_Ring(10,0);
          	  Update_Ring(11,0);
          	  Update_Ring(12,0);
          	  
              //Turn on the the entry LED ring and start the water pump
          	  Fade_Ring(11,ring_brightness,120);
          	  Ball_Washer(BW2,BW2_FAN_SPEED,0);
          	  
          	  //Update tracker and set a delay of 1.76ms (220 * 8ms per count32 increment)
          	  //Update tmark as we will be using Time_Check() to provide the delay
          	  tracker = 5;
          	  delay = 4800;
          	  tmark = count32;
            }
	   	      break;      
    
    //Wait until 1.76s has passed then turn off the water pump and enable the blower fan      
    case 1:
          
	   	    //Check to see if Ball Washer #2 has detected a ball
          if (detection & BW1_EXIT)
          {
          	Fade_Ring(9,0,120);
            Fade_Ring(10,ring_brightness,120);
          }  
    
           if (Time_Check(&tmark,delay))
           {
          	  Fade_Ring(10,0,120);
          	  
             //Enable the blower fan
          	  Ball_Washer(BW1,BW1_FAN_OFF,BW1_PUMP_OFF); 
          	  
          	  //This controls the timeout delay in case the ball gets stuck in the BW
          	  //The delay is set for 2.0s. The timeout delay in the next statement will 
          	  //loop BW_REPEAT amount of times before timing out.
          	  delay = 250;
          	  
          	  BW_ACTIVE = 0;
          	  
          	  //Continue to next statement
          	  tracker = 0;
           }  
	   	     break;      
             
  	case 2:
  	        //Check to see if the ball has passed the exit sensor
            if (detection & BW1_EXIT)
            {	
              //Turn off the entry LED ring and turn on the exit LED ring   	
              Fade_Ring(9,0,80);
	   	        Fade_Ring(10,ring_brightness,120);
	   	        
          	  //Update tracker and set a delay of 2.92s (365 * 8ms per count32 increment)
          	  //Update tmark as we will be using Time_Check() to provide the delay   
          	  tracker = 3;
          	  delay = 1111;
          	  tmark = count32;
            }  
            
           //If the ball does not pass the exit sensor within (2.0s * BW_REPEATS)seconds
           //turn off the LED ring, shut off the blower fan and set a flag
           if (Time_Check(&tmark,delay))
           { 
             repeats++;
                         	 
             //If this timed out it means that the ball never blew out of the pipe
             //and passed the exit sensor. Set a flag and disable the ball washer.
             if (repeats >= BW_REPEATS)
             {
               //Shut off the LED ring and the blower fan	
               Fade_Ring(9,0,80);
            	 Ball_Washer(BW1,BW1_FAN_OFF,BW1_PUMP_OFF); 
            	 
            	 //Set this flag so that the ball washer is disabled since it timed out
  	   	       BW1_JAM = 1;
  	   	       
  	   	       //Reset these to their startup values
  	   	       repeats = 0;
  	   	       tracker = 0;
  	   	      
            	 //The ball washer is no longer active, clear this bit
            	 BW_ACTIVE = 0;
             }  
           }     
	   	     break;           
	   	      
	  //Wait until 2.92s has passed and then start fading the exit LED ring and slow the
    //blower fan speed down. Set delay equal to 640ms and update tracker.
    case 3:
           if (Time_Check(&tmark,delay))
           {
             //Turn off the EXIT LED ring
  	         Fade_Ring(10,0,60);
  	         
  	         //Slow down the ball washer fan. If the ping pong ball has not been
  	         //grabbed yet it will fall back down the pipe and trip the EXIT sensor
	   	       Ball_Washer(BW1,BW1_FAN_LOW_SPEED,BW1_PUMP_OFF); 
	   	       
	   	       //Update tracker and set a delay of 640ms
	   	       tracker = 4;
           	 delay = 320;
          	 tmark = count32;
           }  
	   	      break;  

    //Poll the EXIT sensor for 640ms, if the sensor detects the ping pong ball turn
    //the fan back on at its regular speed so the ball blows back up the pipe, turn
    //the LED ring back on, set the next delay for 2.0s and set the tracker 
    //back to 3. This will loop (n_amount_of_times < BW_REPEATS) before it gives up
    //and turns off the ball washer (setting BW1_JAM) or else the player grabs the 
    //ball before it times out.
    case 4:
           //Check to see if the ball has fallen back down the pipe and passed the 
           //exit sensor. If this has repeated too many times, allow this statement
           //to time out and shut off the ball washer.
           if ((detection & BW1_EXIT) && (repeats < BW_REPEATS))
           {	   	
             //Turn on the exit LED ring 
	   	       Fade_Ring(10,ring_brightness,60);
	   	       
	   	       //Turn the fan speed back up so that the ball will blow up the pipe 
          	 Ball_Washer(BW1,BW1_FAN_SPEED,BW1_PUMP_OFF); 
          	  
          	 //Update tracker and set a delay of 2.0s (250 * 8ms per count32 increment)
          	 //Set tracker to 3 so this will loop back to the last case statement  
          	 tracker = 3;
          	 delay = 1000;
          	 tmark = count32;
          	 
          	 //Increment repeats, if the player never grabs the ping pong ball from the
          	 //ball washer we do NOT want it to run indefinitely. 
          	 repeats++;
           }          
           
           //The operation has either timed out or the player grabbed the ball from
           //the ball washer. Turn off the LED ring and shut off the blower fan. 
           if (Time_Check(&tmark,delay))
           { 
             //Shut off the LED ring and the blower fan
	   	       Fade_Ring(12,0,120);
          	 Ball_Washer(BW1,BW1_FAN_OFF,BW1_PUMP_OFF); 
          	 
          	 //If this timed out it means that the ball fell back down in the pipe,
          	 //set a flag so that we can warn the players that a ball is stuck.
	   	       if (repeats >= BW_REPEATS)
	   	         BW1_JAM = 1;
	   	       
	   	       //Reset these to their startup values
	   	       repeats = 0;
	   	       tracker = 0;
	   	      
          	 //The ball washer is no longer active, clear this bit
          	 BW_ACTIVE = 0;
            }  
	   	      break; 

    //Wait until 1.76ms has passed then turn off the water pump and enable the blower fan                     
    case 5:
              
	   	    //Check to see if Ball Washer #2 has detected a ball
          if (detection & BW2_EXIT)
          {
          	Fade_Ring(11,0,120);
            Fade_Ring(12,ring_brightness,120);
          }  
          
           if (Time_Check(&tmark,delay))
           {
             //Enable the blower fan
          	  Fade_Ring(12,0,120);
          	  Ball_Washer(BW2,BW2_FAN_OFF,BW2_PUMP_OFF); 
          	  
          	  //This controls the timeout delay in case the ball gets stuck in the BW
          	  //The delay is set for 2.0s. The timeout delay in the next statement will 
          	  //loop BW_REPEAT amount of times before timing out.
          	  delay = 1000;
          	  
          	  BW_ACTIVE = 0;
          	  //Continue to next statement
          	  tracker = 0;
           }  
	   	     break;      
             
  	case 6:
  	       //Check to see if the ball has passed the exit sensor
           if (detection & BW2_EXIT)
           {	   	
              //Turn off the entry LED ring and turn on the exit LED ring   
             Fade_Ring(11,0,80);
	   	       Fade_Ring(12,ring_brightness,120);
	   	        
          	 //Update tracker and set a delay of 2.92s (365 * 8ms per count32 increment)
          	 //Update tmark as we will be using Time_Check() to provide the delay   
          	 tracker = 7;
          	 delay = 1065;
          	 tmark = count32;
           }  
            
           //If the ball does not pass the exit sensor within (2.0s * BW_REPEATS)seconds
           //turn off the LED ring, shut off the blower fan and set a flag
           if (Time_Check(&tmark,delay))
           { 
             repeats++;
                         	 
             //If this timed out it means that the ball never blew out of the pipe
             //and passed the exit sensor. Set a flag and disable the ball washer.
             if (repeats >= BW_REPEATS)
             {
               //Shut off the LED ring and the blower fan	
               Fade_Ring(10,0,80);
            	 Ball_Washer(BW2,BW2_FAN_OFF,BW2_PUMP_OFF); 
            	 
            	 //Set this flag so that the ball washer is disabled
  	   	       BW2_JAM = 1;
  	   	       
  	   	       //Reset these to their startup values
  	   	       repeats = 0;
  	   	       tracker = 0;
  	   	      
            	 //The ball washer is no longer active, clear this bit
            	 BW_ACTIVE = 0;
             }  
           }   
	   	     break;        
          
    //Wait until 2.92s has passed and then start fading the exit LED ring and slow the
    //blower fan speed down. Set delay equal to 640ms and update tracker.
    case 7:
           if (Time_Check(&tmark,delay))
           {
             //Turn off the EXIT LED ring
  	         Fade_Ring(12,0,60);
  	         
  	         //Slow down the ball washer fan. If the ping pong ball has not been
  	         //grabbed yet it will fall back down the pipe and trip the EXIT sensor
	   	       Ball_Washer(BW2,BW2_FAN_LOW_SPEED,BW2_PUMP_OFF);  
	   	       
	   	       //Update tracker and set a delay of 640ms
	   	       tracker = 8;
           	 delay = 380;
          	 tmark = count32;
           }  
	   	      break;        
          
    //Poll the EXIT sensor for 640ms, if the sensor detects the ping pong ball turn
    //the fan back on at its regular speed so the ball blows back up the pipe, turn
    //the LED ring back on, set the next delay for 2.0s and set the tracker 
    //back to 7. This will loop (n_amount_of_times < BW_REPEATS) before it gives up
    //and turns off the ball washer (setting BW2_JAM) or else the player grabs the 
    //ball before it times out.
    case 8:
           //Check to see if the ball has fallen back down the pipe and passed the 
           //exit sensor. If this has repeated too many times, allow this statement
           //to time out and shut off the ball washer.
           if ((detection & BW2_EXIT) && (repeats < BW_REPEATS))
           {	   	
             //Turn on the exit LED ring 
	   	       Fade_Ring(12,ring_brightness,60);
	   	       
	   	       //Turn the fan speed back up so that the ball will blow up the pipe 
          	 Ball_Washer(BW2,BW2_FAN_SPEED,BW2_PUMP_OFF); 
          	  
          	 //Update tracker and set a delay of 2.0s (250 * 8ms per count32 increment)
          	 //Set tracker to 7 so this will loop back to the last case statement  
          	 tracker = 7;
          	 delay = 1000;
          	 tmark = count32;
          	 
          	 //Increment repeats, if the player never grabs the ping pong ball from the
          	 //ball washer we do NOT want it to run indefinitely. 
          	 repeats++;
           }          
           
           //The operation has either timed out or the player grabbed the ball from
           //the ball washer. Turn off the LED ring and shut off the blower fan. 
           if (Time_Check(&tmark,delay))
           { 
             //Shut off the LED ring and the blower fan
	   	       Fade_Ring(12,0,120);
          	 Ball_Washer(BW2,BW2_FAN_OFF,BW2_PUMP_OFF); 
          	 
          	 //If this timed out it means that the ball fell back down in the pipe,
          	 //set a flag so that we can warn the players that a ball is stuck.
	   	       if (repeats >= BW_REPEATS)
	   	         BW2_JAM = 1;
	   	       
	   	       //Reset these to their startup values
	   	       repeats = 0;
	   	       tracker = 0;
	   	      
          	 //The ball washer is no longer active, clear this bit
          	 BW_ACTIVE = 0;
            }  
	   	      break; 
	  
	  //This should never execute, if tracker gets changed to a value not listed, reset it to 0 	      
	  default: tracker = 0; 
	           break;                   
  } 
}	
     
/*******************************************************************************
* Function: Check_Pod_Animation(UINT8 *selection)                                                                
*                                                                              
* Variables:                                                                   
* *selection -> Determines which animation is active (Should point to a 4-byte array)                                                                          
*                                                                              
* Description:                                                                 
* This function is used in conjunction with the DA_pods[4] variable, which keeps
* track of which RGB pod animation is currently active. This function will display
* the current RGB pod animation depending on the values of DA_pods[4].                                                                        
*******************************************************************************/  
void Check_Pod_Animation(UINT8 *selection)
{
	static UINT32 tmark = 0;
	
	switch (selection[0])
	{
	  case 0: Cycle_Pod_Animations(); break;
	  case 1:
	  				switch (selection[1])
	  				{
		  				case 0: Pod_Detect(IR_sensors,COLOR[RED],COLOR[GREEN]); break;
		  				case 1: Pod_Detect(IR_sensors,COLOR[WHITE],COLOR[ORANGE]); break;
		  				case 2: Pod_Detect(IR_sensors,COLOR[RED],COLOR[YELLOW]); break;
		  				case 3: Pod_Detect(IR_sensors,COLOR[GREEN],COLOR[PINK]); break; 
		  				case 4: Pod_Detect(IR_sensors,CUSTOM_COLOR1,CUSTOM_COLOR2); break; 
		  			}
		  			
		  			break;
		  			
		case 2:
						switch (selection[1])
						{
							case 0: Color_Throb(COLOR[RED],COLOR[BLUE]);      break;
    					case 1: Color_Throb(COLOR[GREEN],COLOR[VIOLET]);  break;
    					case 2: Color_Throb(COLOR[PINK],COLOR[YELLOW]);   break;
					    case 3: Color_Throb(COLOR[ORANGE],COLOR[WHITE]);  break;	
					    case 4: Color_Throb(CUSTOM_COLOR1,CUSTOM_COLOR2); break;	
					  }   
					  
					  break;
					  
	  case 3: Cycle_Colors(); break;
	  case 4: Pyramid_Chase(); break;
	  case 5:
	  				if (Time_Check(&tmark,2))
	  				{	
		  				switch (selection[1])
		  				{
			  				case 0: Pods_VU_Mode1(VU_signal); 	 break;
			  				case 1: Pods_VU_Mode2(VU_signal[0]); break;
			  			}
			  		}	
		  			
		  			break;
		  		
		case 6: Fade_Pod_Colors(); 											break;	
		case 7: Ripple_Out(400,700);							 			break;	
	  
	  default: 
	  				selection[0] = 0; 
	  				selection[1] = 0; 
	  				selection[2] = 0; 
	  				selection[3] = 0; 
	  				break;
		  					
	}	
}	
     
/*******************************************************************************
* Function: Check_Grid_Animation(UINT8 *selection)                                                                
*                                                                              
* Variables:                                                      
* *selection -> Determines which animation is active (Should point to a 4-byte array)               
*                                                                              
* Description:                                                                 
* This function is used in conjunction with the DA_grid[4] variable, which keeps
* track of which grid animation is currently active. This function will display
* the current LED grid animation depending on the values of DA_grid[4].                                                                        
*******************************************************************************/  
void Check_Grid_Animation(UINT8 *selection)
{
	static UINT32 tmark = 0;
	
	switch (selection[0])
	{
	  case 0: Cycle_Pod_Animations(); 				break;
	  case 1:	Scoreboard(IR_sensors); 				break;
	  case 2:	Pong_Animation(); 							break;
	  case 3:	Checkers();											break;
	  case 4:	Draw_Sine(REGULAR);							break;
	  case 5:	Draw_Sine(INVERTED);						break;
	  case 6:	Dual_Wave(REGULAR);							break;
	  case 7:	Dual_Wave(INVERTED);						break;
	  case 8:	Box_Grid_In();									break;
	  case 9:	Set_Scrolling_Text("Test..");		break;
	  case 10:
	  				if (Time_Check(&tmark,2))
	  				{	
		  				switch (selection[1])
		  				{
			  				case 0: Grid_VU_Mode1(VU_signal[0]);   break;
			  				case 1: Grid_VU_Mode2(VU_signal);   break;
			  			}
			  		}	
		  			
		  			break;	
	  default: 
	  				selection[0] = 0; 
	  				selection[1] = 0; 
	  				selection[2] = 0; 
	  				selection[3] = 0; 
	  				break;
		  					
	}		
}	
     
/*******************************************************************************
* Function: Check_Ring_Animation(UINT8 *selection)                                                                
*                                                                              
* Variables:       
* *selection -> Determines which animation is active (Should point to a 4-byte array)                                                               
*                                                                              
* Description:                                                                 
* This function is used in conjunction with the DA_grid[4] variable, which keeps
* track of which grid animation is currently active. This function will display
* the current LED grid animation depending on the values of DA_grid[4].                                                                        
*******************************************************************************/  
void Check_Ring_Animation(UINT8 *selection)
{
	switch (selection[0])
	{
	  case 0: Ring_Chase(); 									break;
	  case 1:	Crossfade_Rings(); 							break;
	  case 2:	Cycle_Rings();		 							break;
	  	
	  default: 
	  				selection[0] = 0; 
	  				selection[1] = 0; 
	  				selection[2] = 0; 
	  				selection[3] = 0; 
	  				break;
		  					
	}			
}	
     
/*******************************************************************************
* Function: Cycle_Grid_Animations(void)                                                                
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through various grid animations for specific amounts
* of time. Once it has cycled through all of its animations, it will return a 0.
* Otherwise, it will return a 1.                                                                        
*******************************************************************************/                                                                      
UINT8 Cycle_Grid_Animations(void)
{
  static UINT16 delay = 300;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
  static UINT8 count = 0;
      INT8 pod = 0;
    static UINT8 tracker = 0;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[12] == 0xFF)
  {
    seq[12] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[12] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[12];
    tmark = count32;  
    
    //If the sequence is updated, clear the grid
    Clear_Grid(); 
  }
  
  //If a previous 'cup removal detected' animation is not running check the 
  //pod detection states
  if (tracker == 0)
  {  
    //If a pods detection state has been modified it will be saved in 'pod'
    //Otherwise 'pod' will equal 0, indicating no change
    pod = On_Pod_Change(IR_sensors);
   
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 1 & 10, the removed cup was
    //on the MASTER SIDE. Start the 'detected pod removal' animation.
    //if ((pod < 0) && (abs(pod) < 11) && SCROLL_ACTIVE != 1)
    if (0)
    {   
      count++;
     
      if (count > 2)
       count = 0;
      
      switch (count)
      {
        case 0:  tracker = End_Blast(MASTER_SIDE);     break;
        case 1:  tracker = Scrolling_Arrows(SCROLL_GRID_LEFT); break;
        case 2: tracker = Corner_Circles(); break;       
      }
    }   
   
    //If 'pod' is a negative integer it means that the cup has been removed.
    //If the absolute value of 'pod' is between 11 & 20, the removed cup was
    //on the SECONDARY SIDE. Start the 'detected pod removal' animation.
    //else if ((pod < 0) && (abs(pod) < 21) && SCROLL_ACTIVE != 1)
    else if (0)
    {  
      count++;
         
      if (count > 2)
        count = 0;
          
      switch (count)
      {
        case 0:  tracker = End_Blast(SECONDARY_SIDE);     break;
        case 1:  tracker = Scrolling_Arrows(SCROLL_GRID_RIGHT); break;
        case 2: tracker = Corner_Circles(); break;       
      }
    }
   
   //There was no change in the pods detection states, continue original animation  
   else
   {
    //This function will cycle through all of the animations below.
    //More animations can be added by adding a case statement and
    //adjusting the amount of sequences to loop through below this statment.  
    switch (seq[12])
    { 
      case 0: Draw_Sine(REGULAR);  
              delay = TIME_DELAY_10S;
              break;
              
      case 1: Draw_Sine(INVERTED);  
              delay = TIME_DELAY_10S;
              break;    
              
      case 2: seq[12]++;
      				Scoreboard(IR_sensors);  
              delay = TIME_DELAY_30S;
              break;
     
      //Here we set the delay to a very large value as we don't want the
      //interrupt delay check to move on to the next animation until we 
      //know that all of the text is off of the grid.        
      case 3: 
      				Set_Scrolling_Text("CHEXAL BEER PONG TABLES");
      				
      				//If text has finished scrolling, reset scroll flag and set
      				//delay equal to 1 so that the Time_Check(a,b) below advances to 
      				//the next animation	
      				if (SCROLL_FINISHED)
      				{
      				 delay = 1;
      				 SCROLL_FINISHED = 0;
      				} 
      				
      				//If the scroll has not finished yet, set delay to a high value
      				//so that Time_Check(a,b) does not continue to the next animation
      			  else
              delay = TIME_DELAY_1M;  
              
              break;
      
      //Much like the scrolling text, wait until the animation has completed
      //before allowing the function to continue to the next animation        
      case 4: 
              //If the pong animation has completed, allow Time_Check(a,b) to interrupt
              if (Pong_Animation() == 0)    			
                seq[12]++;
              
              //The animation is not over. Make sure that Time_Check(a,b) does not
              //update the seq[x] variable
              else   
              delay = TIME_DELAY_30S;
              
              break;    
              
      case 5: Checkers();  
              delay = TIME_DELAY_10S;
              break;      
              
      case 6: Dual_Wave(REGULAR);  
              delay = TIME_DELAY_10S;
              break;     
              
      case 7: Dual_Wave(INVERTED);  
              delay = TIME_DELAY_10S;
              break;      
              
      case 8: Box_Grid_In();  
              delay = TIME_DELAY_10S;
              break;
    }
   } 
  }  
    

  //A 'detected cup removal' animation has not finished yet. Allow it to finish
  else
  {
    switch (count)
    {
     case 0:  tracker = End_Blast(tracker);     break;
     case 1:  tracker = Scrolling_Arrows(tracker); break;
     case 2: tracker = Corner_Circles(); break;   
    } 
  }   
    
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[12]++;
  
  //If all sequences have been performed, reset all sequences
  //and return a 0 to indicate that the routine is finished
  if (seq[12] > 8)
  {
    //Reset the seq[x] variables that are used by the grid animations
    Reset_Sequences(GRID_SEQUENCES);
    return 0;
  }  
  
  //The function has not finished
  return 1;
}  


/*******************************************************************************
* Function: Cycle_Pod_Animations(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through 10 different animations. More can be added
* by adjusting the code. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                      
*******************************************************************************/
UINT8 Cycle_Pod_Animations(void)
{
  static UINT16 delay = 300;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[13] == 0xFF)
  {
    seq[13] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[13] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[13];
    tmark = count32;  
  }
    
  //This function will cycle through all of the animations below.
  //More animations can be added by adding a case statement and
  //adjusting the amount of sequences to loop through below this statment.  
  switch (seq[13])
  { 
    case 6: Fade_Pod_Colors(); 
            delay = TIME_DELAY_10S;
            break;
  
    case 1: Color_Throb(COLOR[RED],COLOR[BLUE]);
            delay = TIME_DELAY_10S;
            break;
  
    case 2: Color_Throb(COLOR[GREEN],COLOR[VIOLET]);
            delay = TIME_DELAY_10S;
            break;
  
    case 3: Color_Throb(COLOR[PINK],COLOR[YELLOW]);
            delay = TIME_DELAY_10S;
            break;
  
    case 4: Color_Throb(COLOR[ORANGE],COLOR[WHITE]);
            delay = TIME_DELAY_10S;
            break;
            
    case 5: Pyramid_Chase();
            delay = TIME_DELAY_20S;
            break;
  
    case 0: Cycle_Colors();
            delay = TIME_DELAY_20S;
            break;
            
    case 7: Pod_Detect(IR_sensors,COLOR[PINK],COLOR[GREEN]);
            delay = TIME_DELAY_10S;
            break;
            
    case 8: Pod_Detect(IR_sensors,COLOR[WHITE],COLOR[ORANGE]);
            delay = TIME_DELAY_10S;
            break;
            
    case 9: Pod_Detect(IR_sensors,COLOR[YELLOW],COLOR[RED]);
            delay = TIME_DELAY_10S;
            break;
            
    case 10: Ripple_Out(500,800);
            delay = TIME_DELAY_20S;
            break;
  }  
     
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[13]++;
  
  //If all sequences have been performed, reset all sequences
  //and return a 0 to indicate that the routine is finished
  if (seq[13] > 10)
  {
    //Reset the seq[x] variables that are used by the pod animations
    Reset_Sequences(POD_SEQUENCES);
    return 0;
  }  
  
  //The function has not finished
  return 1;
}  


/*******************************************************************************
* Function: Cycle_Pod_Animations(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through 10 different animations. More can be added
* by adjusting the code. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                      
*******************************************************************************/
UINT8 Cycle_Pod_Animations_Sense(void)
{
  static UINT16 delay = 300;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[24] == 0xFF)
  {
    seq[24] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[24] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[24];
    tmark = count32;  
  }
    
  //This function will cycle through all of the animations below.
  //More animations can be added by adding a case statement and
  //adjusting the amount of sequences to loop through below this statment.  
  switch (seq[24])
  {            
    case 0: Pod_Detect(IR_sensors,COLOR[PINK],COLOR[GREEN]);
            delay = TIME_DELAY_10S;
            break;
            
    case 1: Pod_Detect(IR_sensors,COLOR[WHITE],COLOR[ORANGE]);
            delay = TIME_DELAY_10S;
            break;
            
    case 2: Pod_Detect(IR_sensors,COLOR[YELLOW],COLOR[RED]);
            delay = TIME_DELAY_10S;
            break;
            
            
    case 3: Pod_Detect(IR_sensors,COLOR[CYAN],COLOR[YELLOW]);
            delay = TIME_DELAY_10S;
            break;
            
            
    case 4: Pod_Detect(IR_sensors,COLOR[BLUE],COLOR[WHITE]);
            delay = TIME_DELAY_10S;
            break;
  }  
     
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[24]++;
  
  //If all sequences have been performed, reset all sequences
  //and return a 0 to indicate that the routine is finished
  if (seq[24] > 4)
  {
    //Reset the seq[x] variables that are used by the pod animations
    Reset_Sequences(POD_SEQUENCES);
    return 0;
  }  
  
  //The function has not finished
  return 1;
}  

/*******************************************************************************
* Function: Cycle_Ring_Animations(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through 10 different animations. More can be added
* by adjusting the code. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                              
*******************************************************************************/
UINT8 Cycle_Ring_Animations(void)
{
  static UINT16 delay = 300;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[14] == 0xFF)
  {
    seq[14] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[14] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[14];
    tmark = count32;  
  }
    
  //This function will cycle through all of the animations below.
  //More animations can be added by adding a case statement and
  //adjusting the amount of sequences to loop through below this statment. 
  switch (seq[14])
  { 
    case 0: 
    				Cycle_Rings(); 
            delay = TIME_DELAY_30S;
            break;
            
    case 1: 
    				Ring_Chase(); 
            delay = TIME_DELAY_30S;
            break;
            
    case 2: 
    				Crossfade_Rings(); 
            delay = TIME_DELAY_30S;
            break;
  }  
     
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[14]++;
  
  //If all sequences have been performed, reset all sequences
  //and return a 0 to indicate that the routine is finished
  if (seq[14] > 2)
  {
    //Reset the seq[x] variables that are used by the ring animations
    Reset_Sequences(LED_RING_SEQUENCES);
    return 0;
  }  
  
  //The function has not finished
  return 1;
} 


   /*******************************************************************************
* Function: Box_Grid_In(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will create a border around the LED grid and make it 1-pixel wider
* at a time until all of the pixels are on, then it will reverse what it just did 
* until all of the pixels are off. It will then repeat the process making a box in
* animation. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                          
*******************************************************************************/
UINT8 Box_Grid_In(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int delay = 40;  
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[20] == 0xFF)
  {
	  seq[20] = 0;
    last_seq = 0xFF;
  }  
  
  //If a new sequence is starting, update it
  if (last_seq != seq[20])
  {  
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[20];
    tmark = count32;
    
    //Clear grid each time a new circle is to be drawn 
    Clear_Grid(); 
    
    //On each sequence increase the border size up to a total of 6-pixels
    if (seq[20] < 7)
	    Draw_Border(seq[20]);
	  
	  //Now reverse what we just did for the next 6 sequences
	  else
	    Draw_Border(GRID_Y_MAX - seq[20]);
    
    //Update the LED grid
    UPDATE_FRAME();
  } 
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[20]++;
    
  //If the animation has completed reset seq[20] to its default state and
  //return a 0  
  if (seq[20] > GRID_Y_MAX)
  {
    seq[20] = 0xFF;
    return 0;
  }  
  
  //The animation has not completed yet, return a 1
  return 1;  
} 

/*******************************************************************************
* Function: Checkers(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will display a 32x12 checkerboard across the LED grid and alternate
* the state of the LEDs on the checkerboard.  
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                           
*******************************************************************************/         
UINT8 Checkers(void)
{ 
  UINT8 i;
  UINT16 delay = 88;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
	
  //Set variables to start up state if seq[x] is in reset state
  if (seq[16] == 0xFF)
  {
    seq[16] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[16] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[16];
    tmark = count32;
    
    //Display 1st checker pattern on grid
    if (((seq[16]+1) % 2) == 0)
    {
      for (i = 0;i < 12;i++)
      {
        grid_row[i] = 0xAAAAAAAA;
        grid_row[++i] = 0x55555555;
      }  
    }
    
    //Display inverted checker pattern on grid
    else
      Invert_Grid();
    
    //Update the grid
    UPDATE_FRAME();   
  }  
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[16]++;
  
  //If all sequences have been performed, reset seq[x]
  //and return a 1 to indicate that the routine is finished
  if (seq[16] > 19)
  {
    seq[16] = 0xFF;
    return 0;
  }  
  
  //The function has not finished
  return 1;
}

/*******************************************************************************
* Function: Circle_Out(void)                                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will create a circle in the middle of the grid and expland it
* outwards until it passed the borders of the grid.  
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                              
*******************************************************************************/
UINT8 Circle_Out(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int delay = 45;  
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[3] == 0xFF)
  {
	  seq[3] = 0;
    last_seq = 0xFF;
  }  
  
  //Loop 16 times, a radius of 16 is a diameter of 32, which is the full width of
  //the grid.
  if (last_seq != seq[3])
  {  
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[3];
    tmark = count32;
    
    //Clear grid each time a new circle is to be drawn 
    Clear_Grid(); 
    
    //Redraw Pong Border
    Draw_Border(1);
    
    //Draw the size of the circle starting from the center of the grid
    Draw_Circle(15,6,seq[3]); 
    
    //Update the LED grid
    UPDATE_FRAME();
  } 
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[3]++;
    
  //If the animation has completed reset seq[3] to its default state and
  //return a 0  
  if (seq[3] > 16)
  {
    seq[3] = 0xFF;
    return 0;
  }  
  
  //The animation has not completed yet, return a 1
  return 1;  
}  

/*******************************************************************************
* Function: UINT8 Color_Throb(RGB outside_color, RGB inside_color)                                                                 
*                                                                              
* Variables:                                                                   
* outside_color -> This determines the color of the RGB pods on the outside                                                                         
* inside_color -> This determines the color of RGB pods #5 and #15
*                                                                              
* Description:                                                                 
* This function will set all of the RGB pods on each side to one color except
* for the center RGB pod. There will be one outside color that flashes around
* each pod in a triangle fashion and the center will fade in and out with the
* color determined by inside_color.  
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                          
*******************************************************************************/
UINT8 Color_Throb(RGB outside_color, RGB inside_color)
{  
	static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int fade_rate = 270;
  int delay = 140;
  
  //The triangular order of the RGB pods
  UINT8 order[10] = {1,2,4,7,8,9,10,6,3};
	
	//The dimmed outside color
	RGB dim_color; 
	
	//Calculate the dimmed color value for the outside pods
	dim_color.red = outside_color.red / 64;
	dim_color.green = outside_color.green / 64;
	dim_color.blue = outside_color.blue / 64;
	
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[10] == 0xFF)
  {
    seq[10] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[10] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[10];
    tmark = count32;
    
    //Start circulating around the triangle of the RGB pods and lighting
    //each one up full brightness at a time before dimming it again
    if (seq[10] == 0)
    {   
      Fade_Pod(3,dim_color,fade_rate); 
      Fade_Pod(13,dim_color,fade_rate);   
      Fade_Pod(1,outside_color,fade_rate);    
      Fade_Pod(11,outside_color,fade_rate);   
    }  
    
    //Light up the next RGB pod with full brightness while dimming the last 
    //one (which would have been full brightness).
    else
    {
      Fade_Pod(order[seq[10]-1],dim_color,fade_rate);   
      Fade_Pod(order[seq[10]-1]+10,dim_color,fade_rate); 
        
      Fade_Pod(order[seq[10]],outside_color,fade_rate);  
      Fade_Pod(order[seq[10]]+10,outside_color,fade_rate);   
    }  
    
    //Handle the center pods separately from the rest of the pods
    if (seq[10] == 4)
    {
      Fade_Pod(5,inside_color,500);  
      Fade_Pod(15,inside_color,500);  
	  }
	  
	  else if (seq[10] == 8)
	  {
    	//Calculate the dimmed color value for the inside pods
    	dim_color.red = inside_color.red / 32;
    	dim_color.green = inside_color.green / 32;
    	dim_color.blue = inside_color.blue / 32;
	    
	    //Set RGB pods #5 and #15 with their dimmed color
      Fade_Pod(5,dim_color,500);
      Fade_Pod(15,dim_color,500);  
   }   		  
  }  
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[10]++;
  
  //If all sequences have been performed, reset seq[10] and return a 0
  if (seq[10] > 8)
  {
    seq[10] = 0xFF;
    return 0;
  }  	
  
  //If the animation hasn't completed return a 1
  return 1;
}   

/*******************************************************************************
* Function: Corner_Circles(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function is an animation which draws a circle in each corner of the grid.
* The circles then expand across the grid intersecting with one another and 
* making a neat animation. Once the intersected circles pass each other, a circle
* is expanded from the center of the grid to produce a rippling effect. This 
* function will return a 0 if all sequences are completed and the routine is done. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                           
*******************************************************************************/
UINT8 Corner_Circles(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int delay = 40;
  
  int x = 31;
  int y = 11;
  
  //Set variables and the LED grid to a start-up state
  if (seq[5] == 0xFF)
  {
    seq[5] = 0;
    last_seq = 0xFF;
    Clear_Grid();
  } 
  
  //If the seq has changed, update the cycle and change colors
  if (last_seq != seq[5])
  {  
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[5];
    tmark = count32;
    
    //Clear grid for new circle on each sequence lower than 50
    if (seq[5] < 50)
     Clear_Grid(); 
    
    //Re-draw the circles in each corner of the grid
    Draw_Circle(31-x,11-y,seq[5]);
    Draw_Circle(x,11-y,seq[5]);
    Draw_Circle(31-x,y,seq[5]);
    Draw_Circle(x,y,seq[5]);
    
    //Produce the rippling effect by drawing the expanded center circles at 
    //different times in the animation
    if (seq[5] > 21)
     Draw_Circle(15,5,seq[5]-21);
    
    if (seq[5] > 25)
     Draw_Circle(15,5,seq[5]-25);
    
    if (seq[5] > 30)
     Draw_Circle(15,5,seq[5]-30);
    
    if (seq[5] > 36)
     Draw_Circle(15,5,seq[5]-36);
     
    if (seq[5] > 42)
     Draw_Circle(15,5,seq[5]-42);
     
    if (seq[5] > 49)
     Draw_Circle(15,5,seq[5]-49);
    
    if (seq[5] > 54)
    	Fill_Grid();
    
    //Update the LED grid	
    UPDATE_FRAME();	
  } 
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[5]++;
    
  //If all sequences have been performed, reset seq[5]
  //and return a 0 to indicate that the routine is finished
  if (seq[5] > 63)
  {
    seq[5] = 0xFF;
    return 0;
  }  
  
  //Return a 1, indicating that the routine has not ended
  return 1;
} 
/*******************************************************************************
* Function: Crossfade_Rings(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through the LED rings fading them in and out starting
* at opposite ends from each other.   
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                          
*******************************************************************************/
UINT8 Crossfade_Rings(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int delay = 470; 
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[18] == 0xFF)
  {
	  seq[18] = 0;
    last_seq = 0xFF; 
  }  
  
  //If the seq has changed, update the next set of rings that are on opposite
  //sides of the table from each other
  if (seq[18] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[18];
    tmark = count32;
	 
	  //Use the seq[18] to determine which set of rings to update
	  switch (seq[18])
		{
			  case 0:  
  						 //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
							{
								Fade_Ring(9,ring_brightness,RING_FADE_RATE);	
								Fade_Ring(11,ring_brightness,RING_FADE_RATE);
							 }
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[18]++;
							  
								 break;
								 		
			  case 1:  
  						 //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
							{
								Fade_Ring(10,ring_brightness,RING_FADE_RATE);	
								Fade_Ring(12,ring_brightness,RING_FADE_RATE);
							 }
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[18]++;
							  
								 break;			 
			  			 
			  case 2: Fade_Ring(1,ring_brightness,RING_FADE_RATE);	
			  			  Fade_Ring(8,ring_brightness,RING_FADE_RATE); 
			  			  break;
			  			 	
			  case 3: Fade_Ring(4,ring_brightness,RING_FADE_RATE); 	
		 				    Fade_Ring(5,ring_brightness,RING_FADE_RATE); 
		 				    break;	
		 				   
			  case 4: Fade_Ring(2,ring_brightness,RING_FADE_RATE); 	
						    Fade_Ring(7,ring_brightness,RING_FADE_RATE); 
						    break;
				
			  case 5: Fade_Ring(3,ring_brightness,RING_FADE_RATE); 	
						 	  Fade_Ring(6,ring_brightness,RING_FADE_RATE); 
						 	  break;
				
			  case 6: Fade_Ring(13,ring_brightness,RING_FADE_RATE); 	
						 	  Fade_Ring(14,ring_brightness,RING_FADE_RATE); 
						 	  break;
		  
			  case 7:  
  						 //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
							{
								Fade_Ring(9,RING_MIN,RING_FADE_RATE);	
								Fade_Ring(11,RING_MIN,RING_FADE_RATE);
							 }
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[18]++;
							  
								 break;
								 		
			  case 8:  
  						 //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
							{
								Fade_Ring(10,RING_MIN,RING_FADE_RATE);	
								Fade_Ring(12,RING_MIN,RING_FADE_RATE);
							 }
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[18]++;
							  
								 break;		 
			  			 
			  case 9: Fade_Ring(1,RING_MIN,RING_FADE_RATE);	
			  			  Fade_Ring(8,RING_MIN,RING_FADE_RATE); 
			  			  break;
			  			 	
			  case 10: Fade_Ring(4,RING_MIN,RING_FADE_RATE); 	
		 				    Fade_Ring(5,RING_MIN,RING_FADE_RATE); 
		 				    break;	
		 				   
			  case 11: Fade_Ring(2,RING_MIN,RING_FADE_RATE); 	
						     Fade_Ring(7,RING_MIN,RING_FADE_RATE); 
						     break;
				
			  case 12: Fade_Ring(3,RING_MIN,RING_FADE_RATE); 	
						 	   Fade_Ring(6,RING_MIN,RING_FADE_RATE); 
						 	   break;
				
			  case 13: Fade_Ring(13,RING_MIN,RING_FADE_RATE); 	
						 	   Fade_Ring(14,RING_MIN,RING_FADE_RATE); 
						 	   break;
		}							   
  }
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[18]++;
  
  //If all sequences have been performed, reset seq[18] to its default value 
  //and return a 0
  if (seq[18] > 13)
  {
    seq[18] = 0xFF;  
    return 0;
  }
  
  //The animation has not completed yet, return a 1
  return 1;  
}

/*******************************************************************************
* Function: Cycle_Colors(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle each of the pods through each of the default 10 colors.
* It is great to use for testing the RGB LEDs on each pod and to ensure that none
* are faulty. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                            
*******************************************************************************/
UINT8 Cycle_Colors(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0; 
  UINT16 delay = 1820;
  UINT8 i;
    
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[1] == 0xFF)
  {
    seq[1] = 0;
    last_seq = 0xFF;  
  }    
  
  //If the seq has changed, update the cycle and change colors
  if (seq[1] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[1];
    tmark = count32;
    
    //Update pods to the next color defined in RGB COLOR (Globals.h)
		for (i = 0;i < 20;i++)
		  Fade_Pod(i+1,COLOR[seq[1]+1],300);    
  }
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[1]++; 
    
  //If all sequences have been performed, reset seq[1] to its default state and
  //return a 0 indicating the end of the animation
  if (seq[1] > 10)
  {
    seq[1] = 0xFF; 
    return 0;
  }
  
  //If the animation has not finished all of its sequences, return a 1
  return 1;  
}

/*******************************************************************************
* Function: Cycle_Rings(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will cycle through the LED rings fading them in and out. It is
* a good test function to ensure that all rings are working. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                           
*******************************************************************************/
UINT8 Cycle_Rings(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int delay = 200; 
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[2] == 0xFF)
  {
	  seq[2] = 0;
    last_seq = 0xFF; 
  }  
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[2] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[2];
    tmark = count32;
	 
	  //Use the seq[2] to determine which set of rings to update
	  switch (seq[2])
		{
			  case 0:  
  						 //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
							{
								Fade_Ring(9,ring_brightness,RING_FADE_RATE);	
								Fade_Ring(10,ring_brightness,RING_FADE_RATE);
							}
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[2]++;
							  
								 break;			 
			  			 
			case 1:  Fade_Ring(1,ring_brightness,RING_FADE_RATE);	
			  			 Fade_Ring(5,ring_brightness,RING_FADE_RATE); break;
			  			 	
			case 2:  Fade_Ring(2,ring_brightness,RING_FADE_RATE); 	
		 				   Fade_Ring(6,ring_brightness,RING_FADE_RATE); break;	
		 				   
			case 3:  Fade_Ring(3,ring_brightness,RING_FADE_RATE); 	
						   Fade_Ring(7,ring_brightness,RING_FADE_RATE); break;
				
			case 4:  Fade_Ring(4,ring_brightness,RING_FADE_RATE); 	
						 	 Fade_Ring(8,ring_brightness,RING_FADE_RATE); break;
							
		 	case 5:  
  						 //If neither of the ball washers are active update their LED rings
							 if ((BW_ACTIVE == 0))
							 {		 	
								 Fade_Ring(11,ring_brightness,RING_FADE_RATE);	
		  			     Fade_Ring(12,ring_brightness,RING_FADE_RATE); 
		  			   }  
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[2]++;
							     
		  			   break;	
			  			 
		 	case 6:  
  						 //If neither of the ball washers are active update their LED rings
							 if ((BW_ACTIVE == 0))
							 {  
								 Fade_Ring(9,RING_MIN,RING_FADE_RATE);	
			  		     Fade_Ring(10,RING_MIN,RING_FADE_RATE); 
			  	     }
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[2]++;
							  
			  		   break;
				
			case 7:  Fade_Ring(13,ring_brightness,RING_FADE_RATE); 	
						 	 Fade_Ring(14,ring_brightness,RING_FADE_RATE); break;
			  		      
			case 8:  Fade_Ring(1,RING_MIN,RING_FADE_RATE); 	
							 Fade_Ring(5,RING_MIN,RING_FADE_RATE); break;
				
			case 9:  Fade_Ring(2,RING_MIN,RING_FADE_RATE); 	
							 Fade_Ring(6,RING_MIN,RING_FADE_RATE); break;
				
			case 10:  Fade_Ring(3,RING_MIN,RING_FADE_RATE);	
							 Fade_Ring(7,RING_MIN,RING_FADE_RATE); break;
				
			case 11: Fade_Ring(4,RING_MIN,RING_FADE_RATE); 	
							 Fade_Ring(8,RING_MIN,RING_FADE_RATE); break;
							
		  case 12:  
  					   //If neither of the ball washers are active update their LED rings
						   if ((BW_ACTIVE == 0))
					 	   { 
								 Fade_Ring(11,RING_MIN,RING_FADE_RATE);	
		   			     Fade_Ring(12,RING_MIN,RING_FADE_RATE); 
		   			   } 
							 
							 //Ball washer LED rings are in use, continue to the next sequence
							 else
							  seq[2]++;
							    
		   			   break;		
				
			case 13: Fade_Ring(13,RING_MIN,RING_FADE_RATE); 	
							 Fade_Ring(14,RING_MIN,RING_FADE_RATE); break;
			  			 						
		}		   
  }
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[2]++;
  
  //If all sequences have been performed, reset seq[2] to its default value 
  //and return a 0
  if (seq[2] > 13)
  {
    seq[2] = 0xFF;  
    return 0;
  }
  
  //The animation has not completed yet, return a 1
  return 1;  
}

/*******************************************************************************
* Function: Fade_Cups(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will fade the pods in and out the colors red, green, blue and                                                                             
* white. Out of the 4x rows of pods on each side, each row will be a different                                                                             
* color and they will continually fade through each color.   
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                           
*******************************************************************************/
UINT8 Fade_Pod_Colors(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int fade_rate = 500;
  int delay = 1300;
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[4] == 0xFF)
  {
    seq[4] = 0;
    last_seq = 0xFF;
  }  
  
  //This function will rotate through 4 different sequences and color patterns
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[4] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[4];
    tmark = count32;
      
    switch (seq[4])
    {
      case 0: Fade_Pod(20,COLOR[BLUE],fade_rate);      
              Fade_Pod(19,COLOR[BLUE],fade_rate);      
              Fade_Pod(18,COLOR[BLUE],fade_rate);      
              Fade_Pod(17,COLOR[BLUE],fade_rate);      
              Fade_Pod(16,COLOR[RED],fade_rate);       
              Fade_Pod(15,COLOR[RED],fade_rate);       
              Fade_Pod(14,COLOR[RED],fade_rate);       
              Fade_Pod(13,COLOR[GREEN],fade_rate);     
              Fade_Pod(12,COLOR[GREEN],fade_rate);     
              Fade_Pod(11,COLOR[WHITE],fade_rate);    
              Fade_Pod(10,COLOR[BLUE],fade_rate);    
              Fade_Pod(9,COLOR[BLUE],fade_rate);    
              Fade_Pod(8,COLOR[BLUE],fade_rate);    
              Fade_Pod(7,COLOR[BLUE],fade_rate);    
              Fade_Pod(6,COLOR[RED],fade_rate);     
              Fade_Pod(5,COLOR[RED],fade_rate);     
              Fade_Pod(4,COLOR[RED],fade_rate);     
              Fade_Pod(3,COLOR[GREEN],fade_rate);   
              Fade_Pod(2,COLOR[GREEN],fade_rate);   
              Fade_Pod(1,COLOR[WHITE],fade_rate);   
              break;
              
      case 1: Fade_Pod(20,COLOR[RED],fade_rate);       
              Fade_Pod(19,COLOR[RED],fade_rate);       
              Fade_Pod(18,COLOR[RED],fade_rate);       
              Fade_Pod(17,COLOR[RED],fade_rate);       
              Fade_Pod(16,COLOR[GREEN],fade_rate);     
              Fade_Pod(15,COLOR[GREEN],fade_rate);     
              Fade_Pod(14,COLOR[GREEN],fade_rate);     
              Fade_Pod(13,COLOR[WHITE],fade_rate);     
              Fade_Pod(12,COLOR[WHITE],fade_rate);     
              Fade_Pod(11,COLOR[BLUE],fade_rate);     
              Fade_Pod(10,COLOR[RED],fade_rate);     
              Fade_Pod(9,COLOR[RED],fade_rate);     
              Fade_Pod(8,COLOR[RED],fade_rate);     
              Fade_Pod(7,COLOR[RED],fade_rate);     
              Fade_Pod(6,COLOR[GREEN],fade_rate);   
              Fade_Pod(5,COLOR[GREEN],fade_rate);   
              Fade_Pod(4,COLOR[GREEN],fade_rate);   
              Fade_Pod(3,COLOR[WHITE],fade_rate);   
              Fade_Pod(2,COLOR[WHITE],fade_rate);   
              Fade_Pod(1,COLOR[BLUE],fade_rate);    
              break;    
    
      case 2: Fade_Pod(20,COLOR[GREEN],fade_rate);     
              Fade_Pod(19,COLOR[GREEN],fade_rate);     
              Fade_Pod(18,COLOR[GREEN],fade_rate);     
              Fade_Pod(17,COLOR[GREEN],fade_rate);     
              Fade_Pod(16,COLOR[WHITE],fade_rate);     
              Fade_Pod(15,COLOR[WHITE],fade_rate);     
              Fade_Pod(14,COLOR[WHITE],fade_rate);     
              Fade_Pod(13,COLOR[BLUE],fade_rate);      
              Fade_Pod(12,COLOR[BLUE],fade_rate);      
              Fade_Pod(11,COLOR[RED],fade_rate);      
              Fade_Pod(10,COLOR[GREEN],fade_rate);   
              Fade_Pod(9,COLOR[GREEN],fade_rate);   
              Fade_Pod(8,COLOR[GREEN],fade_rate);   
              Fade_Pod(7,COLOR[GREEN],fade_rate);   
              Fade_Pod(6,COLOR[WHITE],fade_rate);   
              Fade_Pod(5,COLOR[WHITE],fade_rate);   
              Fade_Pod(4,COLOR[WHITE],fade_rate);   
              Fade_Pod(3,COLOR[BLUE],fade_rate);    
              Fade_Pod(2,COLOR[BLUE],fade_rate);    
              Fade_Pod(1,COLOR[RED],fade_rate);                     
              break; 
    
      case 3: Fade_Pod(20,COLOR[WHITE],fade_rate);     
              Fade_Pod(19,COLOR[WHITE],fade_rate);     
              Fade_Pod(18,COLOR[WHITE],fade_rate);     
              Fade_Pod(17,COLOR[WHITE],fade_rate);     
              Fade_Pod(16,COLOR[BLUE],fade_rate);      
              Fade_Pod(15,COLOR[BLUE],fade_rate);      
              Fade_Pod(14,COLOR[BLUE],fade_rate);      
              Fade_Pod(13,COLOR[RED],fade_rate);       
              Fade_Pod(12,COLOR[RED],fade_rate);       
              Fade_Pod(11,COLOR[GREEN],fade_rate);    
              Fade_Pod(10,COLOR[WHITE],fade_rate);   
              Fade_Pod(9,COLOR[WHITE],fade_rate);   
              Fade_Pod(8,COLOR[WHITE],fade_rate);   
              Fade_Pod(7,COLOR[WHITE],fade_rate);   
              Fade_Pod(6,COLOR[BLUE],fade_rate);    
              Fade_Pod(5,COLOR[BLUE],fade_rate);    
              Fade_Pod(4,COLOR[BLUE],fade_rate);    
              Fade_Pod(3,COLOR[RED],fade_rate);     
              Fade_Pod(2,COLOR[RED],fade_rate);     
              Fade_Pod(1,COLOR[GREEN],fade_rate);   
              break;    
    }          
  }
    
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[4]++;
  
  //If all sequences have been performed, reset seq[4] and return a 0
  if (seq[4] > 3)
  {
    seq[4] = 0xFF;
    return 0;
  }  
  
  //If the animation has not completed return a 1
  return 1;
}
 

/*******************************************************************************
* Function: LED_Pixel(UINT8 px, UINT8 py, UINT8 state)                                                                     
*                                                                               
* Variables:                                                                    
* px -> The x-coordinate of the pixel
* py -> The y-coordinate of the pixel  
* state -> Determines whether the pixel LED bit will be set to a 1 (ON) or a 0 (OFF)                                                                      
*                                                                               
* Description:                                                                  
* This function will turn the pixel at the (x,y) coordinate off if 'state' is equal to
* 0 or it will turn the pixel on if 'state' is a non-zero value (1).                                                                          
*******************************************************************************/
void LED_Pixel(UINT8 px, UINT8 py, UINT8 state)
{
	//If pixel location is outside of the 32x12 grid, return function  
  if (px >= GRID_X_MAX || py >= GRID_Y_MAX)
    return;
  
  //Clear the pixel that we want to set
  grid_row[py] &= (~((UINT32)1 << px)); 
  
  //Either set the pixel or leave it cleared (dependant on state variable)
  grid_row[py] |= ((UINT32)state << px); 
} 

/*******************************************************************************
* Function: Draw_Border(UINT8 width)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will draw a border along the edges of the LED grid. The width of
* border is determined by the variable 'width'. This function will NOT clear the
* grid, instead it will draw a border around whatever is already drawn on the grid.
* If you need the grid cleared before running this function just call Clear_Grid()
* right before you execute it.                                                                            
*******************************************************************************/
void Draw_Border(UINT8 width)
{
  //Determine how wide the border is and turn on the appropriate pixels
  switch (width)
  {
    case 6: Draw_Rect(5,5,22,2); 
    case 5: Draw_Rect(4,4,24,4); 
    case 4: Draw_Rect(3,3,26,6); 
    case 3: Draw_Rect(2,2,28,8); 
    case 2: Draw_Rect(1,1,30,10); 
    case 1: Draw_Rect(0,0,32,12); break;  
  }  
}

/*******************************************************************************
* Function: Draw_Circle(UINT8 px, UINT8 py, UINT8 radius)                                                                     
*                                                                              
* Variables:                                                                   
* px -> x-coordinate corresponding to center of circle
* py -> y-coordinate corresponding to center of circle 
* radius -> The radius of the circle                                                                
*                                                                              
* Description:                                                                 
* This function will draw a circle at a specified (x,y) location from the center out.
* Once this routine has been ran, a grid update must be called in order to update
* the new LED data to the grid.                                                                             
*******************************************************************************/         
void Draw_Circle(UINT8 px, UINT8 py, UINT8 radius)
{
 INT8 x;
 INT8 y;
 INT8 xchange;
 INT8 ychange;
 INT8 raderr;
 
 //Set the values for the difference between px and py
 x = radius;
 y = 0;
 
 //Calculate the change that is needed in accrodance with the radius
 xchange = 1 - (radius * 2);
 ychange = 1;
 raderr = 0;
 
 //Using the specified radius, calculate and set all of the pixels in their
 //required quadrants
 while (x >= y)
 {
   LED_Pixel(px+x,py+y,ON);
   LED_Pixel(px-x,py+y,ON);
   LED_Pixel(px-x,py-y,ON);
   LED_Pixel(px+x,py-y,ON);
   LED_Pixel(px+y,py+x,ON);
   LED_Pixel(px-y,py+x,ON);
   LED_Pixel(px-y,py-x,ON);
   LED_Pixel(px+y,py-x,ON);
   
   //Update values to determine if the radius size has been met
   y++;
   raderr += ychange;
   ychange += 2;
   
   //Update x coordinate
   if ((raderr * 2) + xchange > 0)
   {
     x--;
     raderr += xchange;
     xchange += 2;
   }
 }
} 
 
/*******************************************************************************
* Function: Draw_Rect(UINT8 px, UINT8 py, UINT8 sx, UINT8 sy)                                                                    
*                                                                              
* Variables:                                                                   
* px -> The x-coordinate where the rectangle will be drawn from                                                                           
* py -> The y-coordinate where the rectangle will be drawn from                                                                              
* sx -> The horizontal length of the rectangle in pixels                                                                          
* sy -> The vertical length of the rectangle in pixels                                                                          
*                                                                              
* Description:                                                                 
* This function will draw a rectangle starting at (px,py) and the size of the
* rectangle will be set by sx and sy.                                                                            
*******************************************************************************/
void Draw_Rect(UINT8 px, UINT8 py, UINT8 sx, UINT8 sy) 
{
  UINT8 i;
  
  //Adjust the size of the rectangle for the loops below
  sx -= 1;
  sy -= 1;
  
  //Draw the x-pixels along the rows first
  for (i = px;i <= px+sx;i++)
  {
    LED_Pixel(i,py,ON); 
    LED_Pixel(i,py+sy,ON); 
  }    
  
  //Draw the y-pixels along the columns second
  for (i = py;i <= py+sy;i++)
  {
    LED_Pixel(px,i,ON); 
    LED_Pixel(px+sx,i,ON); 
  }  
}   	
 
/*******************************************************************************
* Function: Invert_Grid(void)                                                                  
*                                                                              
* Variables:
* N/A                                                                     
*                                                                              
* Description:                                                                 
* This function will invert all of the bits in LED_data. The update grid flag must
* must set after running this in order to display the ivnerted pixels on the grid.                                                                         
*******************************************************************************/
void Invert_Grid(void) 
{
  UINT8 i;
 
 //Clear all bits (LEDs)
 for (i = 0;i < GRID_Y_MAX;i++)
   grid_row[i] = ~grid_row[i];
}     
 
/*******************************************************************************
* Function: Draw_Sine(UINT8 state)                                                                   
*                                                                              
* Variables:                                                                   
* state -> Determines if the sine wave is inverted or not                                                                          
*                                                                              
* Description:                                                                 
* This function will display a sine wave across the LED grid. The sine wave can
* be inverted (0) or display originally (1). Every loop through will modify the
* (x,y) location of the sine wave by one pixel. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                          
*******************************************************************************/
UINT8 Draw_Sine(UINT8 state)   
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
	static float x = 0;
	float dx = 0.27;
	int data[32];
	int amplitude = 5;
  int delay = 20;
	int i;
	
	//Check to see if the program is starting/restarting
  if (seq[6] == 0xFF)
  {
    seq[6] = 0;
    last_seq = 0xFF;
  }  
  
  //If the seq has changed, update the cycle and change colors
  if (last_seq != seq[6])
  {  
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[6];
    tmark = count32;
		
		//Check to see if the inverted sine wave or the original sine wave 
		//has been selected and modify the grid accordingly
		if (state == 0)
	    Fill_Grid();
	  else
	    Clear_Grid();	
	  
	  _T3IE = 0;
	  
	  //Draw a new sine wave across the grid
		for (i = 31;i >= 0;i--)
		{
	  	data[i] = (sin(x*0.7) * amplitude) + 6;
			x += dx;
	    
	    //This will make the sine wave 3-pixels wide; Add or remove more 
	    //pixels to make it wider or thinner
			LED_Pixel(i,data[i]-1,state);
			LED_Pixel(i,data[i],state);
			LED_Pixel(i,data[i]+1,state);
		}
		
	  _T3IE = 1;
		
		//Update the grid
	  UPDATE_FRAME();
	}
		
	//If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[6]++;
    
  //If all sequences have been performed, reset seq[6] to its default value
  //and return a 0
  if (seq[6] > 31)
  {
    //Don't want this value to get too large or we'll have problems, reset periodically
    if (x > 30000)
      x = 0;
      
    seq[6] = 0xFF;
    return 0;
  } 
  
  //The animation has not completed all sequences, return a 1
  return 1;
}

 /*******************************************************************************
* Function: Dual_Wave(UINT8 state)                                                                   
*                                                                              
* Variables:                                                                   
* state -> Determines if the sine wave is inverted or not                                                                          
*                                                                              
* Description:                                                                 
* This function will display two waves across the LED grid. The waves can be
* inverted (0) or display originally (1). Every loop through will modify the
* (x,y) location of the sine waves by one pixel. 
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                            
*******************************************************************************/
UINT8 Dual_Wave(UINT8 state)   
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
	static float x = 0;
	static float x2 = 0;
	float dx = 0.3;
	float dx2 = 0.3;
	int data[32];
	int amplitude = 5;
  int delay = 25;
	int i;
	
	//Check to see if the program is starting/restarting
  if (seq[21] == 0xFF)
  {
    seq[21] = 0;
    last_seq = 0xFF;
  }  
  
  //If the seq has changed, update the cycle and change colors
  if (last_seq != seq[21])
  {  
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[21];
    tmark = count32;
		
		//Check to see if the inverted sine wave or the original sine wave 
		//has been selected and modify the grid accordingly
		if (state == 0)
	    Fill_Grid();
	  else
	    Clear_Grid();	
	  
	  _T3IE = 0;
	  
	  //Draw a new sine wave across the grid
		for (i = 31;i >= 0;i--)
		{
	  	data[i] = (sin(x*0.7) * amplitude) + 6;
			x += dx;
	    
	    //This will make the sine wave 3-pixels wide; Add or remove more 
	    //pixels to make it wider or thinner
			LED_Pixel(i,data[i]-1,state);
			LED_Pixel(i,data[i],state);
			LED_Pixel(i,data[i]+1,state);
		}
	  
	  //Draw a new sine wave across the grid
		for (i = 0;i < 32;i++)
		{
	  	data[i] = (sin(x2*0.7) * amplitude) + 6;
			x2 += dx2;
	    
	    //This will make the sine wave 3-pixels wide; Add or remove more 
	    //pixels to make it wider or thinner
			LED_Pixel(i,data[i]-1,state);
			LED_Pixel(i,data[i],state);
			LED_Pixel(i,data[i]+1,state);
		}
		
	  _T3IE = 1;
		
		//Update the grid
	  UPDATE_FRAME();
	}
		
	//If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[21]++;
    
  //If all sequences have been performed, reset seq[21] to its default value
  //and return a 0
  if (seq[21] > 31)
  {    
	  //Don't want this value to get too large or we'll have problems, reset periodically
    if (x > 30000 || x2 > 30000)
    {
      x = 0;
      x2 = 0;
    }  
      
    seq[21] = 0xFF;
    return 0;
  } 
  
  //The animation has not completed all sequences, return a 1
  return 1;
}	 /*******************************************************************************
* Function: End_Blast(side)                                                                    
*                                                                              
* Variables:                                                                   
* side -> The side of the table that this function will 'shoot' towards                                                                         
*                                                                              
* Description:                                                                 
* This function will display an animation that shoots a line across the BPT
* and is followed by an arrow that turns on all of the LEDs behind it, it will
* then reach the other side and do an 'exploding' animation once it hits.    
* The function takes one parameter 'side' which will determine what side of
* the table the animation should travel towards. If you pass in the constant
* MASTER_SIDE (1), the animation will travel from the secondary side towards the
* master side and end there. The SECONDARY_SIDE (2) constant will travel from the
* master side towards the secondary side. 
*
* This function has to be looped through with the rest of the code in order to
* keep updating the animation. If the animation has finished or has been passed a 
* value other than 1 or 2, it will simply return a 0. If it is in the process of
* completing an animation, it will return the same value as what had been passed into
* it (a 1 (MASTER_SIDE) or a 2 (SECONDARY_SIDE)). This allows us to keep track of 
* the state of this animation in other functions.                                                
*******************************************************************************/
UINT8 End_Blast(UINT8 side)
{ 
  static UINT32 tmark = 0;
  static UINT8 delay = 7;
  static UINT32 temp = 0;
  int i;
  INT8 j;
	
	//If this function is restarting, set up the variables
	if (seq[11] == 0xFF)
	{  	
  	temp = 0;
    seq[11] = 0;
    delay = 7;
  }
  
  //If the animation has finished, reset seq[x] and return a 0 to let the
  //program know that it has finished.    
  if (seq[11] > 89)
  {  
    seq[11] = 0xFF;
    return 0;
  }   
  
  //If the master side has been selected, shoot the animation from the 
  //secondary side TOWARDS the master side.     
  if (side == MASTER_SIDE)
  {    	
     //If the seq has changed, update the new sequence
    if (Time_Check(&tmark,delay))
    { 
      //Reset all of the LED grid data     			  			 
      Clear_Grid();
      
      //Start drawing the two lines from the opposite side of the table
      j = seq[11];
      
      //For each sequence, increase the length of the line by 1 pixel until
      //it reaches the master side			  			 
      for (i = 0;i <= j;i++)
      {
        //Only add to the length of the line once per sequence
        if (j <= 31)
        {
          grid_row[5] = (grid_row[5] >> 1) | 0x80000000;
          grid_row[6] = grid_row[5];
        }  
  		  
  		  //If the rest of the line doesn't need to be updated yet, break
  		  //out of the loop.
  		  else
    		  j = 0xFF; 
  		}	
  		
  		//The next 5 'if' statements just begin drawing circles at
  		//different sequences to provide an 'exploding' effect
  		//Slow the speed in half for these last sequences
  		if (seq[11] > 32)
  		{
        delay = 30;
  		  Draw_Circle(0,5,seq[11] - 32); 
  		}   
  		
  		if (seq[11] > 38)
  		  Draw_Circle(0,5,seq[11] - 37);  
  		
  		if (seq[11] > 42)
  		  Draw_Circle(0,5,seq[11] - 42);  
  		
  		if (seq[11] > 47)
  		  Draw_Circle(0,5,seq[11] - 47);  
  		
  		if (seq[11] > 52)
  		  Draw_Circle(0,5,seq[11] - 52);  
  		
  		//Follow the exploding circles by turning on each row sequentially
  		//behind them.
  		if (seq[11] > 56)
  		{ 
        for (i = 0;i < GRID_Y_MAX;i++)
        {
          if (i == 3 || i == 8 || i == 1 || i == 10)
            continue;
          
          grid_row[i] |= ((UINT32)1 << (seq[11] - 56)) - 1;
        }  
  		}
  		
  		//Update to the next sequence 
  		seq[11]++;
  	
  		UPDATE_FRAME();  
  	}
  	
  	//The animation has not finished yet, return MASTER_SIDE (1)
    return MASTER_SIDE;
  }	
  	
  else if (side == SECONDARY_SIDE)  
  {   	
   //If the seq has changed, update the new sequence
    if (Time_Check(&tmark,delay))
    { 
      //Reset all of the LED grid data     			  			 
      Clear_Grid();
      
      //Start drawing the two lines from the opposite side of the table
      j = 31 - seq[11];
      
      //For each sequence, increase the length of the line by 1 pixel until
      //it reaches the master side			  			 
      for (i = 31;i >= j;i--)
      {
        //Only add to the length of the line once per sequence
        if (j >= 0)
        {
          grid_row[5] = (grid_row[5] << 1) | 0x01;
          grid_row[6] = grid_row[5];
        }  
  		  
  		  //If the rest of the line doesn't need to be updated yet, break
  		  //out of the loop.
  		  else
    		  j = 0xFF; 
  		}	
  		
  		//The next 5 'if' statements just begin drawing circles at
  		//different sequences to provide an 'exploding' effect
  		//Slow the speed in half for these last sequences
  		if (seq[11] > 32)
  		{
        delay = 30;
  		  Draw_Circle(31,5,seq[11] - 32); 
  		}   
  		
  		if (seq[11] > 38)
  		  Draw_Circle(31,5,seq[11] - 37);  
  		
  		if (seq[11] > 42)
  		  Draw_Circle(31,5,seq[11] - 42);  
  		
  		if (seq[11] > 47)
  		  Draw_Circle(31,5,seq[11] - 47);  
  		
  		if (seq[11] > 52)
  		  Draw_Circle(31,5,seq[11] - 52);  
  		
  		//Follow the exploding circles by turning on each row sequentially
  		//behind them.
  		if (seq[11] > 56)
  		{ 
    		temp = (temp >> 1) | 0x80000000;
    		
        for (i = 0;i < GRID_Y_MAX;i++)
        {
          if (i == 3 || i == 8 || i == 1 || i == 10)
            continue;
          
          grid_row[i] |= temp;
        }  
  		}
  		
  		//Update to the next sequence 
  		seq[11]++;
  	
  		UPDATE_FRAME();     
  	}
  	
  	//The animation has not finished yet, return SECONDARY_SIDE (2)
    return SECONDARY_SIDE;
  }    	 
  
  //There is no animation to run, reset seq[x] and return a 0
  else
  {
    seq[11] = 0xFF;
    return 0;
  }    
}  

/*******************************************************************************
* Function: Exploding_Circle(void)                                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will simply draw a circle in the very center of the grid and expand
* it outwards until it expands out of the boundaries of the grid.    
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                            
*******************************************************************************/         
UINT8 Exploding_Circle(void)
{  
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  UINT8 delay = 40;
  UINT8 px;
  UINT8 py;
  
  //Center location of the grid
  px = 15;
  py = 5;
  
  //Reset the function variables to default values
  if (seq[0] == 0xFF)
  {
    seq[0] = 0;
    last_seq = 0xFF;
  }   
    
  //Check to see if the sequence has to be updated
  if (seq[0] != last_seq)
  {
    last_seq = seq[0];
    
    //Clear grid data then write in new data  
    Clear_Grid();
    Draw_Circle(px,py,seq[0]); 
    
    //Update the grid with the new LED data
    UPDATE_FRAME();
   }
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[0]++; 
 
  //If all sequences have been performed, reset seq[0] to its default state
  //and return a 0
  if (seq[0] > 15)
  {
    seq[0] = 0xFF;
    return 0;
  }
  
  //Return a 1 to indiccate that the animation has not finished
  return 1;   
}

/*******************************************************************************
* Function: On_Pod_Change(void)                                                                  
*                                                                              
* Variables:                                                                   
* N/A
*                                 
* Description:                                                                 
* This function will check the sensor data that is passed into it and return a
* 0 if no pods have changed since the previous run. If a pod has changed, the 
* pods numerical value (1 - 20) will be returned from the function and if the returned
* value is a positive value it means that the cup has been added to the table. If
* the returned value is a negative value, it means that the cup has been removed
* from the table. To eliminate false detections, the cup that is detected must
* be consistent for POLL_AMOUNT # of loops, if the sensed values changed before
* the polling times out it will assume that it was a false detect and discard the
* previous data.                                                                      
*******************************************************************************/
INT8 On_Pod_Change(UINT32 sensor_bits)
{
  static UINT32 last_sensor_values = 0x00000000;
  static INT8 saved_pod = 0;
  static UINT16 pod_counter = 0;
  UINT8 i;
  UINT32 change;
  
  //See if a pod has been removed or added to the table
  change = sensor_bits ^ last_sensor_values;
  
  //If the values have changed from the last loop through, reset
  //the variables used for polling the IR sensors
  if (change)
  {
  	pod_counter = 0;
  	saved_pod = 0;
  }	
  
  //If a cup has been removed, return the pods # as a negative integer  
  if (change &  ~sensor_bits)
  {
	  //A pod has been modified, start the polling counter
	  pod_counter = 1;
	  
    //Find which pod has been changed (The lower the pod #, the higher
    //the priority it has).
    for (i = 0;i < 20;i++)
    {
      //Find the pod that has been modified and return the data that will
      //equal the pods number. The value will be a negative integer to 
      //inform us that the cup has been removed from the table.
      if ((change >> i) & 0x00000001)
        saved_pod = (-(i+1));
    }
  }  
  
  //If a pod has been added, return the pods # as a positive integer    
  else if (change & sensor_bits)
  {
	  //A pod has been modified, start the polling counter
	  pod_counter = 1;
	  
    //Find which pod has been changed (The lower the pod #, the higher
    //the priority it has).    
    for (i = 0;i < 20;i++)
    {
      //Find the pod that has been modified and return the data that will
      //equal the pods number. The value will be a positive integer to inform
      //us that the cup has been added from the table.
      if ((change >> i) & 0x00000001)
        saved_pod = (i+1);
    }
  }        
  
  //Update the variable that we use to keep track of the sensor values  
  last_sensor_values = sensor_bits;  
  
  //If the polling has started for a sensor, increment it each time
 	if (pod_counter >= 1)
		pod_counter++;
	
	//If the polling amount has been reached, reset the polling counter and
	//return the value of the pod along with the detection state
  if (pod_counter > POLL_AMOUNT)
  {
	  pod_counter = 0;
	  return saved_pod;
	}

  //No pods have changed, return 0 
  return 0;   
}  

/*******************************************************************************
* Function: Pod_Detect(UINT32 detection, RGB off_color, RGB on_color)                                                               
*                                                                              
* Variables:                                                                   
* detection -> This variable should contain the sensor_detect data from Check_Sensors()                                                                            
* off_color -> The color that you want the RGB pod to be when no cup is detected                                                                             
* on_color -> The color that you want the RGB pod to be when a cup is detected                                                                                
*                                                                              
* Description:                                            
* This function will adjust the color of each RGB pod. The pods color will be set
* to 'off_color' if no cup is detected and 'on_color' if a cup is detected.                                                                        
*******************************************************************************/
void Pod_Detect(UINT32 detection, RGB off_color, RGB on_color)
{
  UINT8 i;
  
  //Cycle through all 20 pods
  for (i = 0;i < 20;i++)
  {
    //If a cup is detected on the specified pod then set the pod to on_color
    if ((detection >> i) & (UINT32)0x00000001)
      Fade_Pod(i+1,on_color,POD_DETECT_FADERATE);
      
    //If a cup is not detected on the specified pod then set the pod to off_color
    else
      Fade_Pod(i+1,off_color,POD_DETECT_FADERATE);
  }   
}   

/*******************************************************************************
* Function: Pyramid_Chase(void)                                                                  
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will slowly fade the RGB pods through different colors starting
* from the top of the pyramid to the bottom of the pyramid.      
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                         
*******************************************************************************/
UINT8 Pyramid_Chase(void)
{  
	static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
	static UINT8 i = 1;
  int fade_rate = 650;
  int delay = 450;
	
  //Check to see if the animation is just starting/restarting and set variables
  //to start-up values
  if (seq[9] == 0xFF)
  {
    seq[9] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the new sequence
  if (seq[9] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[9];
    tmark = count32;
    
    //Update the corresponding pods on each side of the table
    Fade_Pod(seq[9]+1,COLOR[i],fade_rate); 
    Fade_Pod(seq[9]+11,COLOR[i],fade_rate); 
  }  
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[9]++;
  
  //If all sequences have been performed, reset the variables and increment
  //'i' so that a new color will be used on the next run
  if (seq[9] > 9)
  {
	  i++;
    seq[9] = 0xFF;
    
    //If 'i' is greater than 10 reset it to 1
    i = (i > 10) ? 1 : i;
    
    //Once one complete sequence has completed return a 0
    return 0;
  }  	
  
  //The function hasn't completed a full sequence yet
  return 1;
}  

/*******************************************************************************
* Function: Ring_Chase(void)                                                                  
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This animation will light all of the LED rings dimly except for one which will
* be at 'max' brightness. Every sequence the bright LED ring will dim and the next
* LED ring in the square will be lit up to full brightness. This function will not
* modify the ball washer LED rings if BW_ACTIVE is equal to 1.       
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                      
*******************************************************************************/
UINT8 Ring_Chase(void)
{
  static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
  int min = 28;  
  int delay = 140; 
  int fade_rate = 200;
  int i;
  
  //This is all of the LED rings in a square
  int order[12] = {9,1,2,3,4,12,11,8,7,6,5,10};
  
	Fade_Ring(13,65535,400);
	Fade_Ring(14,65535,400); 
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (seq[19] == 0xFF)
  {
	  seq[19] = 0;
    last_seq = 0xFF; 
  }  
  
  //If the seq has changed, update the ring that corresponds to the sequence
  if (seq[19] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[19];
    tmark = count32;
    
	  //Cycle through the rings
	  for (i = 0;i < 12;i++)
	  {
  	  //If none of the ball washers are active, modify them when it is their turn
  	  //Otherwise skip the four ball washer LED rings at locations 0, 5, 6 and 11
		  if (!((BW_ACTIVE == 1) && ((i == 0) || (i == 5) || (i == 6) || (i == 11))))   
	    {
  	    //If it is not the LED rings sequence, make sure that it is dimmed
	  	  if (i != seq[19])
	  	    Fade_Ring(order[i],min,fade_rate);
	  	  
	  	  //If it is this LED rings sequence, put it to 'max' brightness  
	  	  else
	  	    Fade_Ring(order[i],ring_brightness,fade_rate);
	    }	    
	  }      	    						   
  }
  
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[19]++;
  
  //If all sequences have been performed, reset seq[19] to its default value 
  //and return a 0
  if (seq[19] > 11)
  {
    seq[19] = 0xFF;  
    return 0;
  }
  
  //The animation has not completed yet, return a 1
  return 1;  
}

/*******************************************************************************
* Function: Ripple_Out(UINT16 fade_rate, UINT16 delay)                                                                    
*                                                                              
* Variables:                                                                   
* fade_rate -> This adjusts the rate of fade for the RGB pods                                                                            
* delay -> This adjusts the amount of delay between each color                                                                            
*                                                                              
* Description:                                                                 
* This function will set the middle RGB pods (5 & 15) to a color first and
* then the surrounding pods will slowly fade into the same color. The process
* will then repeat with a new color producing a rippling effect.     
*
* This animation must be continually looped through with the main code. If the 
* animation finishes, the function will return a 0. If the animation is still in
* progress it will return a 1.                                                                          
*******************************************************************************/
UINT8 Ripple_Out(UINT16 fade_rate, UINT16 delay)
{  
	static UINT8 last_seq = 0xFF;
  static UINT32 tmark = 0;
	
  //Check to see if the animation is starting/restarting
  if (seq[8] == 0xFF)
  {
    seq[8] = 0;
    last_seq = 0xFF;    
  }
  
  //Check to see if a new sequence has to be updated
  if (seq[8] != last_seq)
  {
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[8];
    tmark = count32;
    
    //This statement determines what colors and RGB pods fade at what sequences
    switch (seq[8])
    {
      case 0:  
              Fade_Pod(5,COLOR[RED],fade_rate);  
              Fade_Pod(15,COLOR[RED],fade_rate);   
              break;
              
      case 1: 
              Fade_Pod(20,COLOR[RED],fade_rate);  
              Fade_Pod(19,COLOR[RED],fade_rate);    
              Fade_Pod(18,COLOR[RED],fade_rate);    
              Fade_Pod(17,COLOR[RED],fade_rate);       
              Fade_Pod(16,COLOR[RED],fade_rate);    
              Fade_Pod(14,COLOR[RED],fade_rate);     
              Fade_Pod(13,COLOR[RED],fade_rate);   
              Fade_Pod(12,COLOR[RED],fade_rate);   
              Fade_Pod(11,COLOR[RED],fade_rate); 
              Fade_Pod(10,COLOR[RED],fade_rate);  
              Fade_Pod(9,COLOR[RED],fade_rate);    
              Fade_Pod(8,COLOR[RED],fade_rate);    
              Fade_Pod(7,COLOR[RED],fade_rate);       
              Fade_Pod(6,COLOR[RED],fade_rate);    
              Fade_Pod(4,COLOR[RED],fade_rate);     
              Fade_Pod(3,COLOR[RED],fade_rate);   
              Fade_Pod(2,COLOR[RED],fade_rate);   
              Fade_Pod(1,COLOR[RED],fade_rate);   
              break;
              
      case 10:  
              Fade_Pod(5,COLOR[ORANGE],fade_rate); 
              Fade_Pod(15,COLOR[ORANGE],fade_rate);   
              break;
              
      case 11: 
              Fade_Pod(20,COLOR[ORANGE],fade_rate);  
              Fade_Pod(19,COLOR[ORANGE],fade_rate);    
              Fade_Pod(18,COLOR[ORANGE],fade_rate);    
              Fade_Pod(17,COLOR[ORANGE],fade_rate);       
              Fade_Pod(16,COLOR[ORANGE],fade_rate);    
              Fade_Pod(14,COLOR[ORANGE],fade_rate);     
              Fade_Pod(13,COLOR[ORANGE],fade_rate);   
              Fade_Pod(12,COLOR[ORANGE],fade_rate);   
              Fade_Pod(11,COLOR[ORANGE],fade_rate);   
              Fade_Pod(10,COLOR[ORANGE],fade_rate);  
              Fade_Pod(9,COLOR[ORANGE],fade_rate);    
              Fade_Pod(8,COLOR[ORANGE],fade_rate);    
              Fade_Pod(7,COLOR[ORANGE],fade_rate);       
              Fade_Pod(6,COLOR[ORANGE],fade_rate);    
              Fade_Pod(4,COLOR[ORANGE],fade_rate);     
              Fade_Pod(3,COLOR[ORANGE],fade_rate);   
              Fade_Pod(2,COLOR[ORANGE],fade_rate);   
              Fade_Pod(1,COLOR[ORANGE],fade_rate);   
              break;
              
      case 12:  
              Fade_Pod(5,COLOR[CYAN],fade_rate); 
              Fade_Pod(15,COLOR[CYAN],fade_rate);   
              break;
              
      case 13:        
              Fade_Pod(20,COLOR[CYAN],fade_rate);  
              Fade_Pod(19,COLOR[CYAN],fade_rate);    
              Fade_Pod(18,COLOR[CYAN],fade_rate);    
              Fade_Pod(17,COLOR[CYAN],fade_rate);       
              Fade_Pod(16,COLOR[CYAN],fade_rate);    
              Fade_Pod(14,COLOR[CYAN],fade_rate);     
              Fade_Pod(13,COLOR[CYAN],fade_rate);   
              Fade_Pod(12,COLOR[CYAN],fade_rate);   
              Fade_Pod(11,COLOR[CYAN],fade_rate);   
              Fade_Pod(10,COLOR[CYAN],fade_rate);  
              Fade_Pod(10,COLOR[CYAN],fade_rate);  
              Fade_Pod(9,COLOR[CYAN],fade_rate);    
              Fade_Pod(8,COLOR[CYAN],fade_rate);    
              Fade_Pod(7,COLOR[CYAN],fade_rate);       
              Fade_Pod(6,COLOR[CYAN],fade_rate);    
              Fade_Pod(4,COLOR[CYAN],fade_rate);     
              Fade_Pod(3,COLOR[CYAN],fade_rate);   
              Fade_Pod(2,COLOR[CYAN],fade_rate);   
              Fade_Pod(1,COLOR[CYAN],fade_rate);   
              break;
              
      case 8:  
              Fade_Pod(5,COLOR[VIOLET],fade_rate);  
              Fade_Pod(15,COLOR[VIOLET],fade_rate);   
              break;
              
      case 9:         
              Fade_Pod(20,COLOR[VIOLET],fade_rate);  
              Fade_Pod(19,COLOR[VIOLET],fade_rate);    
              Fade_Pod(18,COLOR[VIOLET],fade_rate);    
              Fade_Pod(17,COLOR[VIOLET],fade_rate);       
              Fade_Pod(16,COLOR[VIOLET],fade_rate);    
              Fade_Pod(14,COLOR[VIOLET],fade_rate);     
              Fade_Pod(13,COLOR[VIOLET],fade_rate);   
              Fade_Pod(12,COLOR[VIOLET],fade_rate);   
              Fade_Pod(11,COLOR[VIOLET],fade_rate);   
              Fade_Pod(10,COLOR[VIOLET],fade_rate); 
              Fade_Pod(10,COLOR[VIOLET],fade_rate);  
              Fade_Pod(9,COLOR[VIOLET],fade_rate);    
              Fade_Pod(8,COLOR[VIOLET],fade_rate);    
              Fade_Pod(7,COLOR[VIOLET],fade_rate);       
              Fade_Pod(6,COLOR[VIOLET],fade_rate);    
              Fade_Pod(4,COLOR[VIOLET],fade_rate);     
              Fade_Pod(3,COLOR[VIOLET],fade_rate);   
              Fade_Pod(2,COLOR[VIOLET],fade_rate);   
              Fade_Pod(1,COLOR[VIOLET],fade_rate);   
              break;
              
      case 2:  
              Fade_Pod(5,COLOR[GREEN],fade_rate);   
              Fade_Pod(15,COLOR[GREEN],fade_rate);   
              break;
              
      case 3:         
              Fade_Pod(20,COLOR[GREEN],fade_rate);  
              Fade_Pod(19,COLOR[GREEN],fade_rate);    
              Fade_Pod(18,COLOR[GREEN],fade_rate);    
              Fade_Pod(17,COLOR[GREEN],fade_rate);       
              Fade_Pod(16,COLOR[GREEN],fade_rate);    
              Fade_Pod(14,COLOR[GREEN],fade_rate);     
              Fade_Pod(13,COLOR[GREEN],fade_rate);   
              Fade_Pod(12,COLOR[GREEN],fade_rate);   
              Fade_Pod(11,COLOR[GREEN],fade_rate);   
              Fade_Pod(10,COLOR[GREEN],fade_rate); 
              Fade_Pod(10,COLOR[GREEN],fade_rate);  
              Fade_Pod(9,COLOR[GREEN],fade_rate);    
              Fade_Pod(8,COLOR[GREEN],fade_rate);    
              Fade_Pod(7,COLOR[GREEN],fade_rate);       
              Fade_Pod(6,COLOR[GREEN],fade_rate);    
              Fade_Pod(4,COLOR[GREEN],fade_rate);     
              Fade_Pod(3,COLOR[GREEN],fade_rate);   
              Fade_Pod(2,COLOR[GREEN],fade_rate);   
              Fade_Pod(1,COLOR[GREEN],fade_rate);   
              break;
              
      case 6:  
              Fade_Pod(5,COLOR[WHITE],fade_rate); 
              Fade_Pod(15,COLOR[WHITE],fade_rate);   
              break;
              
      case 7:         
              Fade_Pod(20,COLOR[WHITE],fade_rate);  
              Fade_Pod(19,COLOR[WHITE],fade_rate);    
              Fade_Pod(18,COLOR[WHITE],fade_rate);    
              Fade_Pod(17,COLOR[WHITE],fade_rate);       
              Fade_Pod(16,COLOR[WHITE],fade_rate);    
              Fade_Pod(14,COLOR[WHITE],fade_rate);     
              Fade_Pod(13,COLOR[WHITE],fade_rate);   
              Fade_Pod(12,COLOR[WHITE],fade_rate);   
              Fade_Pod(11,COLOR[WHITE],fade_rate);   
              Fade_Pod(10,COLOR[WHITE],fade_rate); 
              Fade_Pod(10,COLOR[WHITE],fade_rate);  
              Fade_Pod(9,COLOR[WHITE],fade_rate);    
              Fade_Pod(8,COLOR[WHITE],fade_rate);    
              Fade_Pod(7,COLOR[WHITE],fade_rate);    
              Fade_Pod(6,COLOR[WHITE],fade_rate);     
              Fade_Pod(4,COLOR[WHITE],fade_rate);     
              Fade_Pod(3,COLOR[WHITE],fade_rate);   
              Fade_Pod(2,COLOR[WHITE],fade_rate);   
              Fade_Pod(1,COLOR[WHITE],fade_rate);   
              break;
              
      case 4:  
              Fade_Pod(5,COLOR[BLUE],fade_rate);   
              Fade_Pod(15,COLOR[BLUE],fade_rate);   
              break;
              
      case 5:         
              Fade_Pod(20,COLOR[BLUE],fade_rate);  
              Fade_Pod(19,COLOR[BLUE],fade_rate);    
              Fade_Pod(18,COLOR[BLUE],fade_rate);    
              Fade_Pod(17,COLOR[BLUE],fade_rate);       
              Fade_Pod(16,COLOR[BLUE],fade_rate);    
              Fade_Pod(14,COLOR[BLUE],fade_rate);     
              Fade_Pod(13,COLOR[BLUE],fade_rate);   
              Fade_Pod(12,COLOR[BLUE],fade_rate);   
              Fade_Pod(11,COLOR[BLUE],fade_rate);   
              Fade_Pod(10,COLOR[BLUE],fade_rate); 
              Fade_Pod(10,COLOR[BLUE],fade_rate);  
              Fade_Pod(9,COLOR[BLUE],fade_rate);    
              Fade_Pod(8,COLOR[BLUE],fade_rate);    
              Fade_Pod(7,COLOR[BLUE],fade_rate);    
              Fade_Pod(6,COLOR[BLUE],fade_rate);      
              Fade_Pod(4,COLOR[BLUE],fade_rate);     
              Fade_Pod(3,COLOR[BLUE],fade_rate);   
              Fade_Pod(2,COLOR[BLUE],fade_rate);   
              Fade_Pod(1,COLOR[BLUE],fade_rate);   
              break;
              
      case 14:  
              Fade_Pod(5,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(15,COLOR[MAGENTA],fade_rate);   
              break;
              
      case 15:         
              Fade_Pod(20,COLOR[MAGENTA],fade_rate);  
              Fade_Pod(19,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(18,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(17,COLOR[MAGENTA],fade_rate);       
              Fade_Pod(16,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(14,COLOR[MAGENTA],fade_rate);     
              Fade_Pod(13,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(12,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(11,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(10,COLOR[MAGENTA],fade_rate); 
              Fade_Pod(10,COLOR[MAGENTA],fade_rate);  
              Fade_Pod(9,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(8,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(7,COLOR[MAGENTA],fade_rate);    
              Fade_Pod(6,COLOR[MAGENTA],fade_rate);     
              Fade_Pod(4,COLOR[MAGENTA],fade_rate);     
              Fade_Pod(3,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(2,COLOR[MAGENTA],fade_rate);   
              Fade_Pod(1,COLOR[MAGENTA],fade_rate);   
              break;
    }
  }  
      
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[8]++;
  
  //If all sequences have been performed, reset seq[8] and return a 0
  if (seq[8] > 15)
  {
    seq[8] = 0xFF;
    return 0;
  }    
  
  //If the animation hasn't finished, return a 1
  return 1;      	
}	


/*******************************************************************************
* Function: Scoreboard(UINT32 pod_sensors)                                                                  
*                                                                              
* Variables:                                                                   
* pod_sensors -> The data from Check_Sensors() which determines the score                                                                          
*                                                                              
* Description:                                                                 
* This function will take the data containing the IR pod sensor values and
* determine the score for each side of the table. It will then draw a scoreboard
* on the LED grid and display the master sides score and the secondary sides score.
* Each pod counts as 1 point, so each team starts out at 10 points. The first team
* to get their opponent down to 0 wins the match.                                                                    
*******************************************************************************/ 
void Scoreboard(UINT32 sensor_bits)
{
  UINT8 i;
  char temp_str[2];
  char master = 0;
  char secondary = 0;
  static UINT32 timer = 0;
  
  //Add up the score for each side
	for (i = 0;i < 10;i++)
	{
	  master += ((sensor_bits >> i) & 0x01);
	  secondary += ((sensor_bits >> (i+10)) & 0x01);
	} 
  
  //Update the scoreboard if the score has changed since the last loop through
  if (Time_Check(&timer,50))
  {
	  timer = count32;
	  
	  //Clear the grid data
	  Clear_Grid();
	  
	  //If the master side has 10 points, adjust the text location and display it	
	  if (master == 10)
		  Set_Text(2,2,"10");
		
		//If the master side is below 10 there is only one charcter. Center the digit.  
		else  
		{
  		//Convert the single numeral to a character (Refer to ASCII chart for help)
			temp_str[0] = master + 0x30;
			temp_str[1] = '\0';
			
			//Set the masters score
		  Set_Text(5,2,temp_str);
		}  
		
	  //If the secondary side has 10 points, adjust the text location and display it	
	  if (secondary == 10)
		  Set_Text(18,2,"10");
		  
		//If the secondary side is below 10 there is only one charcter. Center the digit. 
		else  
		{
  		//Convert the single numeral to a character (Refer to ASCII chart for help)
			temp_str[0] = secondary + 0x30;
			temp_str[1] = '\0';
			
			//Set the secondarys score
		  Set_Text(21,2,temp_str);
		}  
		
		//Draw two dividing lines down the center of the table and draw a 1-pixel border 
		for (i = 0;i < GRID_Y_MAX;i++)
		  grid_row[i] |= 0x00018000; 	
		  
		Draw_Border(1);
		
		//Update the LED grid
		UPDATE_FRAME();
	}
}      

/*******************************************************************************
* Function: Set_Scrolling_Text(char text[64])                                                                    
*                                                                              
* Variables:                                                                   
* text[64] -> Text that is to be displayed on the grid                                                                          
*                                                                              
* Description:                                                                 
* This function will begin writing text to the LED grid. It will only start the
* first column (ROW31) and then set an interrupt flag. The interrupt routine  
* will then handle the rest of the text and scroll it across the table.                                                                          
*******************************************************************************/    
UINT8 Set_Scrolling_Text(char text[64])
{
  UINT8 i;
  
  //If there is a current scrolling operation active, return an error
  if (SCROLL_ACTIVE | SCROLL_FINISHED)
    return 1;
  
  //Get the length of the string
 	str_length = strlen(text);
 	
  //If there is no characters or the characters exceed 63, return an error
	if (str_length == 0 || str_length > 63)
     return 2;
     
  //Copy all of the text into our global text array 'global_string[64]'  
	for (i = 0;i < str_length;i++)
	   global_str[i] = text[i];
  
  //Reset the index variable to keep track of the location in the array
  str_index = 0;
  
  //Clear the rows of the grid
  Clear_Grid();
  
  //Activate the scrolling operation
  SCROLL_ACTIVE = 1;
  
  //Function executed with no errors, return SUCCESS
  return 0;
} 

/*******************************************************************************
* Function: Set_Text(UINT8 px, UINT8 py, char text[12])                                                                  
*                                                                              
* Variables:                                                                   
* px -> Starting x-coordinate where the text is placed                         
* py -> Starting y-coordinate where the text is placed                                                                                      
* text[12] -> The text that is to be displayed                                                                         
*                                                                              
* Description:                                                                 
* This function will write text onto the LED grid array starting at location (px,py).                                                                      
*******************************************************************************/  
void Set_Text(UINT8 px, UINT8 py, char text[12])
{
  UINT8 i;
  UINT8 j;
  UINT8 length;
  UINT16 lookup;
  
  //Get the length of the string
  length = strlen(text);
  	
  //Display each character across the grid
	for (i = 0;i < length;i++)
	{	
		//Track the placement of each letter
		j = px + (i * 6);
		
		//Return the function if the text spans over the limit of the LED grid
		//Each character is in a 5x7 format, ROW26 + 5 Rows for the character
		//will exceed the size of the grid.
		if (j > 26)
		{
			UPDATE_FRAME();
			return;
		}	
				
	  //Find the starting location in the array of Font_5x7[] for the specified character
  	lookup = (text[i] - 32) * 7;
  	
  	//Display the character on the grid
	  grid_row[py]   |= ((UINT32)font_5x7[lookup] << j);
	  grid_row[py+1] |= ((UINT32)font_5x7[++lookup] << j);
	  grid_row[py+2] |= ((UINT32)font_5x7[++lookup] << j);
	  grid_row[py+3] |= ((UINT32)font_5x7[++lookup] << j);
	  grid_row[py+4] |= ((UINT32)font_5x7[++lookup] << j);	
	  grid_row[py+5] |= ((UINT32)font_5x7[++lookup] << j);	
	  grid_row[py+6] |= ((UINT32)font_5x7[++lookup] << j);	
	}	 
} 

/*******************************************************************************
* Function:                                                                    * 
*                                                                              *
* Variables:                                                                   *
* N/A                                                                          *
*                                                                              *
* Description:                                                                 *
*                                                                              *
*******************************************************************************/
UINT8 Scrolling_Arrows(UINT8 direction)
{  
  UINT8 mid;
  UINT8 delay = 40;	
  static UINT8 count = 0;
  static UINT32 tmark = 0;
  static UINT8 last_seq = 0xFF;
  
  mid = GRID_Y_MAX / 2;
  
  //Check to see if the animation is just starting/restarting and set variables
  //to start-up values
  if (seq[22] == 0xFF)
  {
    seq[22] = 0;
    last_seq = 0xFF;    
  }
  
  //If the seq has changed, update the new sequence
  if (seq[22] != last_seq)
  {    
    //Update the sequence and 'tmark' which is used for timing
    last_seq = seq[22];
    tmark = count32;

    if (direction == SCROLL_GRID_LEFT)
    {
      Shift_Grid_Left(1);
      
      grid_row[mid-1-seq[22]] |= 0x80000000;
      grid_row[mid+seq[22]] |= 0x80000000;
    }
    
    else
    {
      Shift_Grid_Right(1);
      
      grid_row[mid-1-seq[22]] |= 0x00000001;
      grid_row[mid+seq[22]] |= 0x00000001;
    }  
       
    UPDATE_FRAME();                 
  }
        
  //If the specified delay has elapsed, continue to the next sequence
  if (Time_Check(&tmark,delay))
    seq[22]++;  
      
  //If all sequences have been performed, reset the variables and increment
  //'i' so that a new color will be used on the next run
  if (seq[22] >= (mid - 1))
  {
    count++;
    seq[22] = 0xFF;
    
    //Once one complete sequence has completed return a 0
    if (count > 20)
    {
      count = 0;
      return 0;
    }  
  }  
     
  //The function hasn't completed a full sequence yet
  return direction;                        
} 	

/*******************************************************************************
* Function: Update_Text(void)                                                                   
*                                                                              
* Variables:   
* N/A                                                                                                                                        
*                                                                              
* Description:                                                                 
* This function will update the location of the scrolling text on the LED grid
* and display it. It is set up in the Timer3 interrupt routine.                                                                             
*******************************************************************************/ 
UINT8 Update_Text(void)
{
  UINT8 i;
  UINT16 loc;
  UINT16 scroll_delay = 45;
  
  static UINT8 j = 0;
  static UINT32 tmark = 0;
  
  //Check to see if the specified amount of time has elapsed
  if (Time_Check(&tmark,scroll_delay))
  {
    //Update the counter for the next operation
    tmark = count32; 
    
    //If the index has been set above 223, finish scrolling the last of the text
    if (str_index > 223)
    { 
      //Check to see if all of the text has been scrolled off of the grid
      if (str_index == 224)
      {
        //Reset the scrolling flag
        SCROLL_ACTIVE = 0;
        
        SCROLL_FINISHED = 1;
        
        //Return value which indicates scrolling has completed
        return 0;
      }  
      
      //Shift the characters to the left again
      Shift_Grid_Left(1); 
      
      //Decrease the count which keeps track of the shifts left til the end of the grid     
      str_index--; 
      
      //Update the LED grid
      UPDATE_FRAME();    
      
      //Return value which indicates scroll operation is still in progress
      return 1;  
    }     
    
    //Check to see if the current character has been scrolled on to the grid
    if (j < 5)
    {  
      //Shift text one location to the left
      Shift_Grid_Left(1);
      
      //Calculate the start location for the current letter in the font array
      loc = (global_str[str_index] - 32) * 7; 
      
      //Loop through each of the rows that will contain bits of the current character
      for (i = 0;i < 7;i++)
      {
        //Start each character at the far right location of the LED grid
        if (font_5x7[loc+i] & (1 << j)) 
          grid_row[i+2] |= 0x80000000; 
      }  
      
      //Increment j to read the next columns of bits pertaining to the current character  
      j++;
    }
    
    //The last bit is completely on the grid, get the next character
    else
    {
      //Reset the starting column
      j = 0;
      
      //Index to the next character in the string
      str_index++;
      
      //If we are at the end of the string, the index variable will be used to keep track of
      //the last remaining 32 left shifts in order to shift the text completely off the grid
      if (global_str[str_index] == '\0')
        str_index = 0xFF;
      
      //Put a space between the characters  
      Shift_Grid_Left(1);
    }
    
    //Update the LED grid
    UPDATE_FRAME();
  }  
  
  //Return a scroll in progress value
  return 2;    
} 

/*******************************************************************************
* Function: Pong_Game(void)                                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will draw a 1-pixel border around the LED grid, thus providing                                                                             
* a 30x10 grid inside of this border. It will then draw a specified amount of                                                                             
* balls and have them bounce around the table for a specified amount of time                                                                             
* before the program will remove one ball and increase the sizes of the 
* remaining balls. Once it gets to the last ball and finishes all of the steps
* in the program it will call the 'Circle_Out()' function to provide an
* explosion like animation. This function is able to draw a maximum of 10
* balls on the grid at once. The amount that will actually be drawn is equal
* to the value of 'ball_amt'.                                                                          
*******************************************************************************/
UINT8 Pong_Animation(void)
{ 
  static PONG_BALL BALL[10];
  static UINT32 tmark = 0;
  static INT16 step = 0;
  static INT16 step2 = 0;
  static UINT8 f_pong = 0;
  static UINT8 ball_amt = BALL_AMOUNT;
  UINT8 i,j;
  
  //This is used to set the direction of each BALL at the start of the program
  //These are random, we don't want the balls to bounce in a pattern with each other
  UINT8 sx[10] = {1,-1,-1,1,1,-1,1,-1,1,-1};
  UINT8 sy[10] = {1,-1,1,-1,-1,-1,1,-1,1,-1};
  
  //This is used to set the xy location of each BALL at the start of the program
  //These are random chosen locations
  UINT8 px[10] = {23,5,26,8,12,2,14,28,19,25};
  UINT8 py[10] = {7,3,2,5,1,6,4,9,8,5};
  
  //If this flag is cleared, the function is just beginning. Set all variables
  //back to default values.
  if (!f_pong)
  { 
    //Initalize all pong balls to their default values  
    for (i = 0;i < ball_amt;i++)
    {
      BALL[i].radius = BALL_RAD;
      BALL[i].speed_x = sx[i];
      BALL[i].speed_y = sy[i];
      BALL[i].new_x = px[i];
      BALL[i].new_y = py[i];
    }
    
    //Set flag so that the function will begin  
    f_pong = 1;
  }
  
  //On first run, this will always prove true. After the first loop through
  //it will only loop through once the time interval has passed and the next
  //step can be executed.
  if (Time_Check(&tmark,GAME_SPEED))
  {
    //Update the sequence, clear the grid and redraw the border but
    //do not update the grid as we will update all of it at once at
    //the end of this function.
    step++;
    Clear_Grid();
    Draw_Border(1);
    
    //After each ball changes locations on the grid 50x times continue to
    //the next step. The value 50 is what determines how long the balls
    //will bounce around on the table before the next step.
    if ((step % 50) == 0)
    {
      //Continue to next step, remove one more ball
      step2++;
      ball_amt--;
      
      //Increase each remaining balls radius by 1 in size
      for (i = 0;i < ball_amt;i++)
      {
        BALL[i].radius = 1 + (step2 >> 1);
        
        //If the ball is on a border and its radius gets increased by 1
        //it will glitch out and get stuck on the border. A quick way to
        //solve this is to move it 1-pixel horizontally and vertically
        //towards the center of the grid every time the radius increases. 
        if (BALL[i].new_x > 16)
        	BALL[i].new_x--; 
        else
        	BALL[i].new_x++; 
        
        if (BALL[i].new_y > 5)
        	BALL[i].new_y--; 
        else
        	BALL[i].new_y++; 
        
      }  
    }    
    
    //Once the time has elapsed for the last ball on the table, perform the
    //'Circle_Out()' animation and clear f_pong to initiate the end of the function
    if (step2 == 7)
    { 
      step2 = 0;    
      
      //Wait for function to finish before continuing
      while (Circle_Out());
      
      //Clear flag
      f_pong = 0;
      ball_amt = BALL_AMOUNT;
      step2 = 0;
      step = 0;
      
      //Return 0 for operation successful
      return 0;
    }  
      
    //Update the direction of the balls and their locations
    for (i = 0;i < ball_amt;i++)
    {
      BALL[i].old_x = BALL[i].new_x;
      BALL[i].old_y = BALL[i].new_y;
      BALL[i].new_x += BALL[i].speed_x;
      BALL[i].new_y += BALL[i].speed_y;
      
      //If the ball has hit a border, change its direction
      if ((BALL[i].new_x - (BALL[i].radius)) < 0)
       BALL[i].speed_x = -BALL[i].speed_x;
       
      //If the ball has hit a border, change its direction
      if ((BALL[i].new_x + (BALL[i].radius)) > GRID_X)
       BALL[i].speed_x = -BALL[i].speed_x;
      
      //If the ball has hit a border, change its direction
      if ((BALL[i].new_y - (BALL[i].radius)) < 0)
       BALL[i].speed_y = -BALL[i].speed_y;
       
      //If the ball has hit a border, change its direction
      if ((BALL[i].new_y + (BALL[i].radius)) > GRID_Y)
       BALL[i].speed_y = -BALL[i].speed_y;
      
      //Draw the ball in its new location
      for (j = 0;j < BALL[i].radius;j++)
       Draw_Circle(BALL[i].new_x,BALL[i].new_y,j);
    }   
    
    //Prepare to update the grid
    UPDATE_FRAME();
  }  
  
  //Return 1 as function hasn't completed
  return 1;
}   

/*******************************************************************************
* Function: Reset_All_Sequences(UINT8 mode)                                                                    
*                                                                              
* Variables:                                                                   
* mode -> Determines which seq[x] variables should be set to their default values                                                                          
*                                                                              
* Description:                                                                 
* This function will reset all of the seq[] variables used for animations back to 
* their default state of 0xFF.                                                                     
*******************************************************************************/
void Reset_Sequences(UINT8 mode)
{
	UINT16 i;
	
	//Refer to Globals.h to see which seq[x] variable belongs to which function
	switch (mode)
	{
  	//Reset all seq[] variables to their default state
  	case ALL_SEQUENCES:
  	
        	  for (i = 0;i < SEQ_AMOUNT;i++)
          		 seq[i] = 0xFF;
          	
            break;
            
    //Reset all seq[x] variables that are used for LED grid animations        
    case GRID_SEQUENCES:
            seq[0]  = 0xFF;
            seq[3]  = 0xFF;
            seq[5]  = 0xFF;
            seq[6]  = 0xFF;
            seq[11] = 0xFF;
            seq[12] = 0xFF;
            seq[16] = 0xFF;
            seq[20] = 0xFF;
            seq[21] = 0xFF;
            seq[22] = 0xFF;
            seq[24] = 0xFF;
            break;
            
    //Reset all seq[x] variables that are used for RGB pod animations      
    case POD_SEQUENCES:
            seq[1]  = 0xFF;
            seq[4]  = 0xFF;
            seq[8]  = 0xFF;
            seq[9]  = 0xFF;
            seq[10] = 0xFF;
            seq[13] = 0xFF;
            seq[24] = 0xFF;
            break;
            
    //Reset all seq[x] variables that are used for LED Ring animations      
    case LED_RING_SEQUENCES:
            seq[2]  = 0xFF;
            seq[14]  = 0xFF;
            seq[18]  = 0xFF;
            seq[19]  = 0xFF;
            break;
  }           
}

#endif
