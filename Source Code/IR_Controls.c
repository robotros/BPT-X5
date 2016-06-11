/*******************************************************************************
* Title: IR_Controls.c                                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                      
*                                                                              
* Description:                                                                 
* This file holds the functions and routines that are used to process and sort the
* IR codes received from the TSOP4838 Infrared Receiver module.                                                                
*******************************************************************************/

#ifndef IR_CONTROLS_C
#define IR_CONTROLS_C

#include "Main_Includes.h"
#include "IR_Controls.h"
#include "LED_Control.h"
#include "RS232_Setup.h"
#include "LCD_Setup.h"
#include "MSGEQ7_Setup.h"
#include "Delay_Setup.h"
#include <stdio.h>
#include <stdlib.h>

extern volatile UINT8 pod_brightness;

extern volatile T16_FLAG FLAG1;
extern volatile UINT16 RINGn[16];

extern volatile UINT32 NEC_code;
extern volatile UINT8 keypress;
extern volatile UINT32 count32;

/*******************************************************************************
* Function: Parse_IR_Data(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function is called when the IR receiver detects a signal and will
* decode the signal into an NEC format. The decoded IR code will then be stored
* in a global 32-bit unsigned integer variable. If the received code cannot be
* decoded properly, it will simply discard it and reset the tracking variables.                                                                            
*******************************************************************************/
void Parse_IR_Data(void)
{
  UINT16 t1 = 0;
  UINT16 t2 = 0;
  UINT16 period = 0;
  
	static UINT8 tracker = 0;
  static UINT32 code = 0;
  static UINT32 last_code;
  static UINT16 bit_cnt = 0;
  static UINT32 key_timer = 0;
  static UINT16 last_capture = 0;
  
  //If there has been more than 80ms key_timer between keypresses, start fresh
 	if ((key_timer + 80) < count32)
 	{
	 	//Ensure that any old data in the buffer is read; Prevents overflow
		if (IC1CON1bits.ICBNE)
			t2 = IC1BUF; 
			
	 	//Ensure that any old data in the buffer is read; Prevents overflow
		if (IC1CON1bits.ICBNE)
			t2 = IC1BUF; 
			
	 	//Ensure that any old data in the buffer is read; Prevents overflow
		if (IC1CON1bits.ICBNE)
			t2 = IC1BUF; 
			
	 	//Reset counting variables to their default states
	  tracker = 0;
	 	key_timer = count32;
	}	 		
	
	//Check to see if the interrupt is at the start of a new keypress		
  if (tracker == 0)
  { 
	  //Set up the timing variables
	  t1 = 0;
	  
	  //This may have already been read in the above'if' statement
	  //If it hasn't been read, save the capture value
		if (IC1CON1bits.ICBNE)
			t2 = IC1BUF; 
		
		//Reset the bit counting timer
		bit_cnt = 0;
		
		//Reset the key code value
		code = 0;
		
		//Begin capturing bits on the following interrupts
		tracker++;
	} 
  
  //Check for the start bit
  else if (tracker == 1)
  { 
	  //Save the last falling edge capture
	  t1 = last_capture;
	  
	  //Save the current falling edge capture
	  t2 = IC1BUF;  
	  
	  //Store the current capture for the next capture interrupt
		last_capture = t2;
	  
	  //If the capture value has not overflowed, calculate the period
	  if (t2 > t1)
	  	period = t2 - t1;
	  	
	  //If the capture value has overflowed, calculate the peiod w/ the overflow
	  else
	  	period = (65535 - t1) + t2; 
  	
  	//If the capture period is longer than 12.8ms, the start bit has been received
  	if (period > 3500)
			tracker++;
	  
  	//If the capture period is longer than 10.98ms and less than 12.8ms, the user
  	//is holding the key down and has not released it
		else if (period > 3000)
		{
		 //Disable the input capture module
  	 _IC1IE = 0;
		 
		 //Reset the tracking variable
		 tracker = 0;
		 
		 //They key is being held, the key being pressed is the same as the previous key
		 NEC_code = last_code;
		 
		 //Alert the program that a key has been pressed
	   UPDATE_IR_CMD = 1; 
	   
	   //Exit the function		
		 return;	
		}	
		
		//A false keypress was detected, discard it	
		else 
		  tracker = 0;
	} 
	
	//Parse the IR data into 32 bits of data
	else 
	{	
	  //Save the last falling edge capture
	  t1 = last_capture;
	  
	  //Save the current falling edge capture
	  t2 = IC1BUF;  
	  
	  //Store the current capture for the next capture interrupt
		last_capture = t2;
	  
	  //If the capture value has not overflowed, calculate the period
	  if (t2 > t1)
	  	period = t2 - t1;
	  	
	  //If the capture value has overflowed, calculate the peiod w/ the overflow
	  else
	  	period = (65535 - t1) + t2;
		
		//If the period is longer than 1.82ms, change the specified bit to a 1
  	if (period > 550)
      code |= (UINT32)((UINT32)1 << (31 - bit_cnt));
		
		//If all 32 bits have been received, set a flag and disable the interrupt	
		if (bit_cnt == 31)
		{
			//Disable the input capture interrupt 
  	  _IC1IE = 0;
  	  
  	  //Reset the tracking variable
			tracker = 0;
			
			//Save the keypress code into a global variable
		  NEC_code = code;
		  
		  //Update the last_code variable
		  last_code = NEC_code;	
		  
		  //Set a flag	    
		  UPDATE_IR_CMD = 1; 	
		  
		  //Exit the function	   
		  return;	  
		} 
		
		//If a bit is invalid (high for too long), reset the operation 
		else if (period > 1000)
		{
			//Make sure that the input capture interrupt is still enabled			
  		_IC1IE = 1;
  		
  		//Reset the tracking variable
		  tracker = 0;
		  
		  //Exit the function
		  return;
		}  
		
		//Increment the bit counter 
  	bit_cnt++;	
  }   
}  

/*******************************************************************************
* Function: Check_Remote(UINT32 code)                                                                   
*                                                                              
* Variables:                                                                   
* code -> Stores the 32-bit received NEC code                                                                         
*                                                                              
* Description:                                                                 
* This function will compare a 32-bit NEC code to the codes which correspond to
* each of the 24 buttons on the IR remote to see if there is a match. If there
* is a match, it will update the global variable 'keypress' to store the key
* that was pressed. Otherwise, it will save a KEY_UNRECOGNIZED (0) value in the
* variable 'keypress', indicating that a non-matching code was received.                                                                             
*******************************************************************************/
void Check_Remote(UINT32 code)
{   
	//Compare the received NEC code
  switch (code)
  {
    case CUSTOM_REMOTE_A1:
    case STOCK_REMOTE_A1: 	keypress = KEY_UP; 				break;
    		 
    case CUSTOM_REMOTE_A2:
    case STOCK_REMOTE_A2: 	keypress = KEY_DOWN;	 		break;  
                       
    case CUSTOM_REMOTE_A3:
    case STOCK_REMOTE_A3: 	keypress = KEY_OFF;	 			break;  
                       
    case CUSTOM_REMOTE_A4:
    case STOCK_REMOTE_A4: 	keypress = KEY_ON;				break; 
                       
    case CUSTOM_REMOTE_B1:
    case STOCK_REMOTE_B1: 	keypress = KEY_LEFT;	 		break;  
                      
    case CUSTOM_REMOTE_B2:
    /*case STOCK_REMOTE_B2:*/ 	keypress = KEY_RIGHT;	 		break;  
                      
    case CUSTOM_REMOTE_B3:
    case STOCK_REMOTE_B3: 	keypress = KEY_SD; 				break;  
                       
    case CUSTOM_REMOTE_B4:
    case STOCK_REMOTE_B4: 	keypress = KEY_VU; 				break;
    
    case CUSTOM_REMOTE_C1:
    case STOCK_REMOTE_C1: 	keypress = KEY_ENTER; 		break;
    
    case CUSTOM_REMOTE_C2:
    case STOCK_REMOTE_C2: 	keypress = KEY_CANCEL; 		break;
    
    case CUSTOM_REMOTE_C3:
    case STOCK_REMOTE_C3: 	keypress = KEY_MODE; 			break;
    
    case CUSTOM_REMOTE_C4:
    case STOCK_REMOTE_C4: 	keypress = KEY_SETTINGS; 	break; 
      
    case CUSTOM_REMOTE_D1:
    case STOCK_REMOTE_D1: 	keypress = KEY_NUM1; 			break; 
      
    case CUSTOM_REMOTE_D2:
    case STOCK_REMOTE_D2: 	keypress = KEY_NUM2; 			break;  
     
    case CUSTOM_REMOTE_D3:
    case STOCK_REMOTE_D3: 	keypress = KEY_NUM3; 			break;  
     
    case CUSTOM_REMOTE_D4:
    case STOCK_REMOTE_D4: 	keypress = KEY_SCORE; 		break; 
      
    case CUSTOM_REMOTE_E1:
    case STOCK_REMOTE_E1: 	keypress = KEY_NUM4; 			break; 
      
    case CUSTOM_REMOTE_E2:
    /*case STOCK_REMOTE_E2:*/  	keypress = KEY_NUM5; 			break; 
      
    case CUSTOM_REMOTE_E3:
    case STOCK_REMOTE_E3: 	keypress = KEY_NUM6; 			break;  
     
    case CUSTOM_REMOTE_E4:
    case STOCK_REMOTE_E4: 	keypress = KEY_CALIBRATE; break;   
    
    case CUSTOM_REMOTE_F1:
    case STOCK_REMOTE_F1: 	keypress = KEY_NUM7; 			break; 
      
    case CUSTOM_REMOTE_F2:
    /*case STOCK_REMOTE_F2:*/ 	keypress = KEY_NUM8; 			break;   
    
    case CUSTOM_REMOTE_F3:
    case STOCK_REMOTE_F3: 	keypress = KEY_NUM9; 			break;  
     
    case CUSTOM_REMOTE_F4:
    case STOCK_REMOTE_F4: 	keypress = KEY_NUM0; 			break; 
    
    //If no match is found, return an error  
    default: 
    					keypress = KEY_UNRECOGNIZED;
    					printf("Unrecognized Input: 0x%.8lX \r\n",code); 
    					break;
  }
  
  //Display the NEC code and its respective key
  printf("NEC Code: 0x%.8lX \r\n",code);
  printf("Keypress: %u \r\n \n",keypress);
  
  //Clear the update IR flag
  UPDATE_IR_CMD = 0; 
}  

#endif
