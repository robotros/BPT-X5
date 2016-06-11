/*******************************************************************************
* Title: BPT.c                                                                       
* Version: 2.02                                                                  
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                          
*                                                                               
* Description:                                                                  
* This is the main source code file for the BPT X5 platform. It contains the
* main loop and links to source code to control the various features on the
* X5 PCB.                                                                        
*******************************************************************************/

/*************************************************
* Include device file and configuration settings *
*************************************************/
#include "Main_Includes.h"
#include "Configuration.h"
#include "Globals.h"
#include <stdio.h>
#include <stdlib.h>
#include "Delay_Setup.h"
#include "ADC_Setup.h"
#include "PORT_Setup.h"
#include "SPI_Setup.h"
#include "PWM_Setup.h"
#include "RS232_Setup.h"
#include "Timers_Setup.h"
#include "TLC5955_Setup.h"
#include "74HC595_Setup.h"
#include "Interrupts.h"
#include "IR_Sensors.h"
#include "IR_Controls.h"
#include "LCD_Setup.h"
#include "Grid_Setup.h"
#include "SD_Setup.h"
#include "LED_Graphics.h"
#include "LCD_Control.h"
#include "FAT32_Setup.h"
#include "MSGEQ7_Setup.h"
#include "EEPROM_Setup.h"
#include "LED_Control.h"
#include "BT_Functions.h"
#include "VU_Control.h"

/*******************************************************************************
* Function: main()                                                             * 
*                                                                              *
* Variables:                                                                   *
* N/A                                                                          *
*                                                                              *
* Description:                                                                 *
* All of the code in the program is ran from inside this function. This        *
* function will never terminate as long as the MCU has power applied to it.    *
*******************************************************************************/
int main(void)
{   
  UINT16 buf[7];
  
  UINT32 bw_bits;
  UINT32 tmark = 0;
  UINT32 IR_delay = 0;
  
  //Start-up the PIC and configure the oscillator and ports
  Oscillator_Init();  
  PORT_Init();
  
  //Set up the various peripherals used
  Modules_Init(); 
  
  //Enable the 16x2 LCD
  LCD_Init();
  Delay_ms(5);
  
  //Display the main LCD screen
  LCD_Screensaver();
  
    Delay_ms(800);
  //Initialize the UART at 9600bps to initialize the HC06 module
  UART_Init(9600); 
  
  //Initialize the EEPROM
  EEPROM_Init();
  
  //Rename the HC06 module and set the baudrate of the UART to 115200 bps
  HC06_Init("BPT_78785",115200);
  
  //Prepare the LED grid to start receiving data
  HC595_Init();

  //Configure the TLC5955 for operation
  TLC5955_Default_Init(TLC5955_DSPRPT_ON);
  Delay_ms(250); 
  
  //Begin updating the LED grid
  TMR5_Init();
  
  //Allow the pods to detect cups
  Enable_IR_Sensors(TX_MAX_BRIGHTNESS);
  
  //Prepare the ADC to read 4 channels at a time, starting on channel 3
  ADC_CHANNEL(3);
  
  //Calibrate the IR sensors
  Sensor_Calibration();

	//Turn on the underlighting
	RGB_Underlighting(COLOR[GREEN]);

  while (1)
  { 	 			  		 
		//If a keycode has been received from the remote, process the key press
	  if (UPDATE_IR_CMD)
	  {
		  //Enable the input capture interrupt again
  		_IC1IE = 1;
		 	
		 	//Clear the keypress update flag
		 	UPDATE_IR_CMD = 0;
			 	
			//Make sure that enough time has passed between keypresses before decoding them.
			//Without a delay here the remote will record multiple keypresses at once, 
			//resulting in a jittery selection menu
		  if (Time_Check(&IR_delay,320))   
		    Check_Remote(NEC_code);  
		}    	  
	  
	  //Check if a UART command has been received
    if (UPDATE_UART)
    {
	    //Disable the UART interrupt while we are parsing the data
      _U1RXIE = 0;
      
      //Parse the received data looking for a recognized command
      Check_UART_Command(UART_rx_buf);  
      
		  //Clear UART flag
		  UPDATE_UART = 0;
		  
      //Re-enable the UART receive interrupt
      _U1RXIE = 1;
    }
    
    //Check if a valid key has been pressed
    if (keypress)
    {
	    //Process the key press
    	Handle_Key_Command(keypress);
    	keypress = 0;
    }	

	  //Read the IR sensors periodically (every ~20ms or so)
    if (Time_Check(&tmark,20)) 
    {
	    //Retrieve the IR sensor readings
      IR_sensors = Update_All_Sensors();         
    
	    //Mask off the bits that aren't needed for the ball washer IR sensors
	    bw_bits = (IR_sensors >> 20);
	    
	    //If a ball has been detected, have the ball washer start up
	    if (ball_washers == ON)
		    	Ball_Washers_Detect(bw_bits);	  
	   } 
	   
    //If the table is not in standby mode, run the regular animations
    if (MODE_STANDBY == 0)
    {		    
	    //If VU Meter mode isn't selected, cycle through animations
	    if (VU_Meter == 0)
	    {
		    //Cycle through each features animations
	      Cycle_Grid_Animations();
				Cycle_Pod_Animations();
	      Cycle_Ring_Animations();
	    }
	    
	    else if (VU_Meter == 1)
	    {
		    Scoreboard(IR_sensors);
        Cycle_Pod_Animations_Sense();
		    Cycle_Ring_Animations();
		  } 
	    
	    //Enter VU Mode
	    else
	    {
			  //Read all 7 frequency bands from the MSGEQ7
			  MSGEQ7_Read(buf);
			    
			  //Adjust each reading by accounting for offset
		    MSGEQ7_Auto_Adjust(buf,VU_signal);			    
		    
		    //Display the selected VU animations
		    switch (VU_Meter)
		    {
			    case 2:
		    					Grid_VU_Mode1(VU_signal[0]);
		   						Pods_VU_Mode1(VU_signal);
	                Cycle_Ring_Animations();
			    	 			break;
			    	 			
			    case 3: 
		    					Grid_VU_Mode2(VU_signal);
				          Cycle_Pod_Animations();
	                Cycle_Ring_Animations();
		   						//Pods_VU_Mode2(VU_signal[0]);
		   						break;
		   						
			    case 4: 
			    				Bargraph_Update(VU_signal[0]);
		   						Pods_VU_Mode2(VU_signal[0]);
			    				break;

			    
			    //Shouldn't ever execute; Just here as a failsafe				
			    default: VU_Meter = 0; break;
			  } 
			}	  
		}      
	}   
}

/*******************************************************************************
* Function: Timer #1 Interrupt                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This interrupt controls the RGB pods and their updates. It has a period of                                                                               
* 8192uS, updating the pods with a refresh rate of ~120Hz.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
  //Check to see if any TLC channels (pods, LED rings, etc) need to be updated
  if (TLC5955_UPDATE)
  {
	  //Update the PWM outputs and reset the update flag
    TLC5955_Write_GS(TLC_data2);
    TLC5955_UPDATE = 0;
  } 
 
 //Clear the TMR1 interrupt flag
 _T1IF = 0;
}

/*******************************************************************************
* Function: Timer #3 Interrupt                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This timer is set to interrupt every ~1ms and controls the fading of the RGB                                                                               
* pods, as well as the operation of any scrolling text.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{ 
  //Check to see if any TLC channels are fading (RGB pods, rings, etc)
  Fade_State(); 
   
  //If text is scrolling across the LED grid, update the scroll operation 
  if (SCROLL_ACTIVE)
    scroll_status = Update_Text();
 
 //Clear the TMR3 interrupt flag   
 _T3IF = 0;
}

/*******************************************************************************
* Function: Timer #4 Interrupt                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This timer is not currently enabled and is only here as a blank template in                                                                              
* case anyone wants to use it.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
//TMR4 is not used; May be needed for future implementation	 
 _T4IF = 0;
}

/*******************************************************************************
* Function: Timer #5 Interrupt                                                                      
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This timer has the highest priority and interrupts in 1ms intervals. It keeps                                                                              
* track of the global counting variable as well as refreshing the LED grid.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void)
{ 
	//The global counter which is used by the Time_Check(a,b) interrupt delay
	//Increments every ~1ms, will roll over to 0 after 50 days of continuous
	//operation
  count32++;
  
  //Refresh the LED grid
  Grid_Control();
  
 _T5IF = 0;
}

/*******************************************************************************
* Function: UART Rx Interrupt                                                                     
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This interrupt is called upon whenever the PIC receives data from the onboard                                                                              
* bluetooth->UART module.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{    
  INT8 i;
  UINT8 data;
  static UINT32 UART_delay = 0;
  static UINT8 buf_size = 0;
  static UINT16 cnt = 0;
  
  //Clear any overrun errors
  if (_OERR)
  	_OERR = 0;
  
  //Clear any framing errors	
  if (_FERR)
  	_FERR = 0;
  	
  	
	if (Time_Check(&UART_delay,300))   
	{
		cnt = 0;
		buf_size = 0;
  }		
  
  
  data = UART_Rx();
  
  if (UPDATE_UART == 0)
  {	
	  if (cnt < 2)
	  {
		  UART_rx_buf[cnt] = data;
		  
		  if (cnt == 1)
			{
				buf_size = Receive_Buffer_Size(UART_rx_buf);
				
				if (buf_size == 0)
				{
					UPDATE_UART = 1;
					cnt = 0;
					buf_size = 0;
				}
					
				else if (buf_size == BT_INVALID_COMMAND)
				{
					buf_size = 0;
					cnt = 0;
				}	
			} 
			
		  cnt++;
		}
	
		else if ((cnt - 2) < (buf_size - 1))
		{
			UART_rx_buf[cnt] = data;
			cnt++;
		}		
		
		else
		{
			UART_rx_buf[cnt] = data;
			UPDATE_UART = 1;
			cnt = 0;
			buf_size = 0;
		}	
	}
  
  //Reset the UART receive interrupt
  _U1RXIF = 0;
}

/*******************************************************************************
* Function: External Interrupt #0                                                                      
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
* This interrupt handles the IR receiver and decodes the received infrared signals                                                                              
* from the remote. The programming currently pauses operations while it decodes
* the IR signal, but will soon be changed to use the CCP module so that operation
* isn't briefly halted on a keypress.                                                                              
*******************************************************************************/
void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt(void)
{  
//Reset the interrupt (No longer used; This can be deleted)
//  _INT0IF = 0;
}

/*******************************************************************************
* Function:                                                                    
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:  
*                                                                              
*******************************************************************************/
void __attribute__ ((__interrupt__)) _IC1Interrupt(void)
{  	
   _IC1IF = 0; // Reset respective interrupt flag 
    Parse_IR_Data();
}
