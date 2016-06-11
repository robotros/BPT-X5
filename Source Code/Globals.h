/*******************************************************************************
* Title:                                                                       *
* Version: 1.0                                                                 *
* Author: Jeff Nybo                                                            *
* Date:                                                                        *
*                                                                              *
* Description:                                                                 *
*                                                                              *
*******************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

/*************************************************
*               Global Variables                 *
*************************************************/
UINT8 seq[SEQ_AMOUNT];  //SEQ_AMOUNT is set to 50 by default

char uart_str[32];
char global_str[64];

UINT8 DA_grid[4] = {0,0,0,0};
UINT8 DA_pods[4] = {0,0,0,0};
UINT8 DA_rings[4] = {0,0,0,0};

UINT8 _FAR SD_buf[SD_BUF_SIZE];
UINT8 UART_rx_buf[128];

INT8 LCD_pmenu = 0;
INT8 LCD_cmenu = 100;

UINT8 ball_washers = 1;

/***************************************
*                Menus                 *
****************************************
0 -> Screen Saver (Home)

1 -> Adjust Brightness
   0 -> Adjust Pods
   1 -> Adjust Rings
      
2 -> Calibrate Sensors

3 -> SD Card
   0  -> Select Animations
	 1  -> Transfer Files
			
4 -> VU Meter
   0 -> Mode #1
   1 -> Mode #2
   2 -> Mode #3
   3 -> Mode #4
   4 -> Random Modes
  
5 -> Settings
   0 -> Air Baths
   1 -> Firmware Upgrade
   
****************************************
***************************************/   
                        
/***************************************
* The animations that use these seq[n] *
****************************************
seq[0]  - Exploding_Circle()
seq[1]  - Cycle_Colors()
seq[2]  - Cycle_Rings()
seq[3]  - Circle_Out()
seq[4]  - Fade_Cups()
seq[5]  - Corner_Circles()
seq[6]  - Draw_Sine()
seq[7]  - VU_Meter_Bar()
seq[8]  - Ripple_Out()
seq[9]  - Pyramid_Chase()
seq[10] - Color_Throb()
seq[11] - End_Blast()
seq[12] - Cycle_Grid_Animations()
seq[13] - Cycle_Pod_Animations()
seq[14] - Cycle_LED_Ring_Animations()
seq[15] - Cycle_Combined Animations()
seq[16] - Checker()
seq[17] - Ball_Washer_Jam_Error()
seq[18] - Crossfade_Rings()
seq[19] - Ring_Chase()
seq[20] - Box_Grid_In()
seq[21] - Draw_Double_Sine()
seq[22] - Scrolling_Arrows()
seq[23] - Intro_Animation()
seq[24] - Our_Test_Animation()
seq[25] - Cycle_Pod_Animations_Sense()
seq[26] -
.
.
.
seq[49] - 
***************************************/      

UINT32 IR_sensors = 0;
UINT32 count32 = 0;
UINT32 NEC_code;
UINT32 grid_row[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
UINT32 grid_frame[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

INT16 cal_light[24];
UINT16 IR_value[24];

RGB PODn[21];
RGB_FADE RGB_DIFF[21];
RING_FADE RING_DIFF[16];

UINT16 IR_duty;
UINT16 TLC_data[96];
UINT16 TLC_data2[96];
UINT16 RINGn[16];

UINT8 pod_brightness = 20;
UINT16 ring_brightness = 65535;

UINT8 str_index;
UINT8 letter_buffer[7];
UINT8 error_code;
UINT8 str_length;
UINT8 scroll_status;

UINT8 frame_update = 0;

T8_FLAG TLC;
UINT16 tlc_delay;
T32_FLAG fLED;

UINT16 ring_update;
UINT32 pod_update;

T16_FLAG FLAG1;
FILE_SYSTEM FAT32;

UINT8 keypress = 0;

UINT8 VU_Meter = 0x00;

/***************************************
*         FLAG1 designated bits        *
****************************************
0  - TLC5955_UPDATE   (TLC5955_Setup.h)
1  - UPDATE_IR_CMD    (IR_Controls.h)
2  - UPDATE_UART      (BT_Functions.h)
3  - FAT32_PAUSE      (FAT32_Setup.h)
4  - GRID_UPDATE      (LED_Control.h)  
5  - SCROLL_ACTIVE    (LED_Graphics.h)
6  - BW_ACTIVE        (LED_Graphics.h)
7  - DIAGNOSE_ERROR   (LED_Graphics.h)
8  - BW1_JAM          (LED_Graphics.h)
9  - BW2_JAM          (LED_Graphics.h)
10 - SCROLL_FINISHED  (LED_Graphics.h)
11 - MODE_STANDBY     (LED_Control.h)
12 - 
13 - 
14 - 
15 -
***************************************/

const RGB DEFAULT_COLOR[11] = {
                               {0,0,0},            //Black
                               {65535,0,0},         //Red
                               {0,65535,0},         //Green
                               {0,0,65535},         //Blue
                               {0,65535,24575},    //Cyan
                               {65535,0,22000},      //Magenta
                               {65535,25000,0},      //Yellow
                               {65535,0,2000},      //Pink
                               {65535,2800,0},       //Orange
                               {65535,0,30535},      //Violet
                               {65535,45535,55535}   //White  
                              };                 

RGB COLOR[11] = {
                 {0,0,0},            //Black
                 {65535,0,0},         //Red
                 {0,65535,0},         //Green
                 {0,0,65535},         //Blue
                 {0,65535,24575},    //Cyan
                 {65535,0,22000},      //Magenta
                 {65535,25000,0},      //Yellow
                 {65535,0,2000},      //Pink
                 {65535,2800,0},       //Orange
                 {65535,0,30535},      //Violet
                 {65535,45535,55535}   //White  
                };    
                
RGB CUSTOM_COLOR1 = {65535,25000,0};
RGB CUSTOM_COLOR2 = {0,0,65535};    

UINT16 VU_signal[7] = {0,0,0,0,0,0,0};         
                      
const char UART_CMD[25][32] = {"BT",								//0
                               "BT+MENU",						//1
                               "BT+VERSION",				//2
                               "BT+POD",						//3
                               "BT+LEDRING", 				//4 
                               "BT+PIXEL",					//5
                               "BT+STANDBY",				//6
                               "BT+ACTIVE",					//7
                               "BT+HELP",						//8
                               "BT+POD+HELP",				//9
                               "BT+LEDRING+HELP",		//10
                               "BT+PIXEL+HELP",			//11
                               "BT+EEPROM+HELP",		//12
                               "BT+WRITE+EEPROM",		//13
                               "BT+READ+EEPROM",		//14
                               "BT+RESET",					  //15
                               "BT+WRITE+SD",			  //16
                               "BT+READ+SD"			    //17
                              };

#endif
