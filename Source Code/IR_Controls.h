/*******************************************************************************
* Title: IR_Controls.h                                                                    
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                      
*                                                                              
* Description:                                                                 
* This file holds the constants and definitions for the infrared remote control
* (Model: E27 24-Key IR Remote) that is used in conjunction with a TSOP4838 or
* any other 38KHz receiver.
*
* Note:
* There are many different versions of the RGB 24-Key IR Remote that use different
* NEC codes. If you connect up the bluetooth module to a terminal program, when you
* press a key on any infrared remote, the decoded value will be sent to the UART
* terminal from the Master PCB. This way, you can press a button, record its NEC
* code, change the button defintion below to match that NEC code and repeat for
* all other 23 buttons. It's as easy as that to use a different model. Otherwise,
* just use the MR16, E14, E27 or GU10 model of these remotes, which is what the
* NEC codes below are matched too.                                                                                                                   
*******************************************************************************/

#ifndef IR_CONTROLS_H
#define IR_CONTROLS_H

/*************************************************
*                   Constants                    *
*************************************************/
#define IR_RMT              PORTBbits.RB7

#define UPDATE_IR_CMD       FLAG1.b1

#define IR_CODE_ERROR				0xFFFFFFFF

#define CAPTURE_FE					2
#define CAPTURE_RE					3

//NEC Code for each button on the customized remote
#define CUSTOM_REMOTE_A1           0x00FF05FA
#define CUSTOM_REMOTE_A2           0x00FF04FB
#define CUSTOM_REMOTE_A3           0x00FF06F9
#define CUSTOM_REMOTE_A4           0x00FF07F8
#define CUSTOM_REMOTE_B1           0x00FF09F6
#define CUSTOM_REMOTE_B2           0x00FF08F7 //CONFLICT #1
#define CUSTOM_REMOTE_B3           0x00FF0AF5
#define CUSTOM_REMOTE_B4           0x00FF0BF4
#define CUSTOM_REMOTE_C1           0x00FF0DF2
#define CUSTOM_REMOTE_C2           0x00FF0CF3
#define CUSTOM_REMOTE_C3           0x00FF0EF1
#define CUSTOM_REMOTE_C4           0x00FF0FF0
#define CUSTOM_REMOTE_D1           0x00FF15EA
#define CUSTOM_REMOTE_D2           0x00FF14EB
#define CUSTOM_REMOTE_D3           0x00FF16E9
#define CUSTOM_REMOTE_D4           0x00FF17E8
#define CUSTOM_REMOTE_E1           0x00FF19E6
#define CUSTOM_REMOTE_E2           0x00FF18E7 //CONFLICT #2
#define CUSTOM_REMOTE_E3           0x00FF1AE5
#define CUSTOM_REMOTE_E4           0x00FF1BE4
#define CUSTOM_REMOTE_F1           0x00FF11EE
#define CUSTOM_REMOTE_F2           0x00FF10EF //CONFLICT #3
#define CUSTOM_REMOTE_F3           0x00FF12ED
#define CUSTOM_REMOTE_F4           0x00FF13EC

//NEC Code for each button on the stock remote
#define STOCK_REMOTE_A1           0x00FFA05F
#define STOCK_REMOTE_A2           0x00FF20DF
#define STOCK_REMOTE_A3           0x00FF609F
#define STOCK_REMOTE_A4           0x00FFE01F
#define STOCK_REMOTE_B1           0x00FF906F
#define STOCK_REMOTE_B2           0x00FF10EF //CONFLICT #3
#define STOCK_REMOTE_B3           0x00FF50AF
#define STOCK_REMOTE_B4           0x00FFD02F
#define STOCK_REMOTE_C1           0x00FFB04F
#define STOCK_REMOTE_C2           0x00FF30CF
#define STOCK_REMOTE_C3           0x00FF708F
#define STOCK_REMOTE_C4           0x00FFF00F
#define STOCK_REMOTE_D1           0x00FFA857
#define STOCK_REMOTE_D2           0x00FF28D7
#define STOCK_REMOTE_D3           0x00FF6897
#define STOCK_REMOTE_D4           0x00FFE817
#define STOCK_REMOTE_E1           0x00FF9867
#define STOCK_REMOTE_E2           0x00FF18E7 //CONFLICT #2
#define STOCK_REMOTE_E3           0x00FF58A7
#define STOCK_REMOTE_E4           0x00FFD827
#define STOCK_REMOTE_F1           0x00FF8877
#define STOCK_REMOTE_F2           0x00FF08F7 //CONFLICT #1
#define STOCK_REMOTE_F3           0x00FF48B7
#define STOCK_REMOTE_F4           0x00FFC837

//Keypress value for each button on remote
#define KEY_UNRECOGNIZED		0
#define KEY_UP							1
#define KEY_DOWN						2
#define KEY_OFF							3
#define KEY_ON							4
#define KEY_LEFT						5						
#define KEY_RIGHT						6			
#define KEY_SD							7			
#define KEY_VU							8	
#define KEY_ENTER						9			
#define KEY_CANCEL					10	
#define KEY_MODE						11	
#define KEY_SETTINGS				12			
#define KEY_NUM1						13	
#define KEY_NUM2						14	
#define KEY_NUM3					  15		
#define KEY_SCORE						16	
#define KEY_NUM4						17	
#define KEY_NUM5						18	
#define KEY_NUM6						19	
#define KEY_CALIBRATE				20			
#define KEY_NUM7						21	
#define KEY_NUM8						22	
#define KEY_NUM9						23	
#define KEY_NUM0					  24

/*************************************************
*                   Macros                       *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/  
void Update_LCD_Menu(void);
void Check_Remote(UINT32 code);

void Parse_IR_Data(void);

void Fake_IR_Signal(UINT32 signal);

#endif
