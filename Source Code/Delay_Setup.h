/*******************************************************************************
* Title: Delay_Setup.h                                                         
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: April 23, 2014                                                         
*                                                                              
* Description:                                                                 
* This source file contains two types of delays: a delay for milliseconds and  
* a delay for microseconds. This also contains Time_Check(***) which is used for
* interrupt timing.                                                   
*******************************************************************************/
#ifndef DELAY_SETUP_H
#define DELAY_SETUP_H

#define Delay_us(x) __delay32(((x*FCYC)/1000000L))
#define Delay_ms(x) __delay32(((x*FCYC)/1000L)) 
#include <libpic30.h>

UINT8 Time_Check(UINT32 *mark, UINT16 interval);

#endif
