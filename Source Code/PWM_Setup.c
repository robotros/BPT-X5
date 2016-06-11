/*******************************************************************************
* Title: PWM_Setup.c                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the functions that allow the user to setup the PWM modules
* and adjust the duty cycle and period of the PWM module.                      
*******************************************************************************/       
                  
#ifndef PWM_SETUP_C
#define PWM_SETUP_C
      
#include "Main_Includes.h"
#include <HSPWM.h>
#include "PWM_Setup.h"   

/*******************************************************************************
* Function: PWM2_Init(void)                                                    
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function allows the user to initialize the PWM2 module.                 
*******************************************************************************/
void PWM1_Init(UINT16 timebase, UINT16 duty)
{
	//Enable the PWM1 module
  OpenHSPWM(PWM_MOD_EN,PWM_INPUT_CLK_DIV0,timebase,0);

	//Configure the PWM module
  ConfigHSPWM1(PWM_TB_MODE_PER | PWM_D_CYLE_DC | PWM_DT_POS | PWM_PRI_TB,
               PWM_H_PIN_GPIO | PWM_L_PIN_EN, 0, 0, 0);

	//Set the PWM duty cycle
  SetHSPWMDutyCycle1(duty);
  
  //Set PWM to have no deadtime
  SetHSPWMDeadTime1(0,0);
  
  //Set the output polarity
  IOCON3 = 0x0000;
  IOCON2 = 0x0000;
}         
    
#endif
