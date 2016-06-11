/*******************************************************************************
* Title: LED_Graphics.h                                                                     
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                        
*                                                                              
* Description:                                                       
* This file contains various functions that are use to update and control the RGB pods,
* LED grid and LED rings.                                                                             
*******************************************************************************/

#ifndef LED_GRAPHICS_H
#define LED_GRAPHICS_H

/*************************************************
*               Global Variables                 *
*************************************************/
//Tracks text scrolling
#define SCROLL_ACTIVE     FLAG1.b5
//Tracks text scrolling
#define SCROLL_FINISHED     FLAG1.b10

//Used to define scrolling direction in Scrolling_Arrows()
#define SCROLL_GRID_LEFT       1
#define SCROLL_GRID_RIGHT      2

//Reset Sequences
#define ALL_SEQUENCES       0
#define GRID_SEQUENCES      1
#define POD_SEQUENCES       2
#define LED_RING_SEQUENCES  3

//Used in Pod_Detect() to set the faderates of the pods
#define POD_DETECT_FADERATE 40

//The amount of times that the IR sensor will be read and has to be 
//consistent before returning a sensor detected value. Used in
//On_Pod_Change(). This is used to prevent false detections.
#define POLL_AMOUNT         25

//LED Ring max and min values for various animations
//Added fade rate too
#define RING_MAX            65535
#define RING_MIN            0
#define RING_FADE_RATE      400

//Time_Check(*tmark,delay) Delays 
//(Delay in Seconds = value * 0.001s per interrupt)
#define TIME_DELAY_1S       125*8
#define TIME_DELAY_2S       250*8
#define TIME_DELAY_3S       375*8
#define TIME_DELAY_4S       500*8
#define TIME_DELAY_5S       625*8
#define TIME_DELAY_6S       750*8
#define TIME_DELAY_7S       875*8
#define TIME_DELAY_8S       1000*8
#define TIME_DELAY_9S       1125*8
#define TIME_DELAY_10S      1250*8
#define TIME_DELAY_15S      1875*8
#define TIME_DELAY_20S      2500*8
#define TIME_DELAY_30S      3750*8
#define TIME_DELAY_40S      5000*8
#define TIME_DELAY_50S      6250*8
#define TIME_DELAY_1M       7500*8
#define TIME_DELAY_2M       15000*8
#define TIME_DELAY_3M       22500*8
#define TIME_DELAY_4M       30000*8
#define TIME_DELAY_5M       37500*8
#define TIME_DELAY_1H       450000L*8

//Used for Pong_Animation()
typedef struct
{
  INT8 radius;
  INT8 speed_x;
  INT8 speed_y;
  INT8 old_x;
  INT8 old_y;
  INT8 new_x;
  INT8 new_y;
} PONG_BALL;  

/*************************************************
*                   Constants                    *
*************************************************/
//Determines whether the pixels are inverted or not (Draw_Sine(x))
#define INVERTED          0
#define REGULAR           1

//The lower this is, the more sensitive the VU meter is. If you put it at max
//sensitivity you may pick up noise from the circuit itself. This may take a bit
//of trial and error to get things working but these values work great for me.
#define VU_SENSITIVTY     3

//This will adjust the offset of the VU meter. After adjusting VU_SENSITIVITY, 
//the VU meter can be tweaked with this value to cancel any unwanted noise that
//the ADC picks up.
#define VU_OFFSET		      20

//This determines how quick the bar will fall back down in the VU_Meter_Bar()
//function. The lower the value, the quicker it will fall.
#define FLOATING_BAR      7

//This is used to determine if a pods detection
//status has changed in Detect_Pod_Change(sensor_bits)
#define BITMASK_MASTER_SIDE       0x000003FF
#define BITMASK_SECONDARY_SIDE    0x0000FFC0

//Used to select the side for certain animations such as End_Blast()
#define MASTER_SIDE       1
#define SECONDARY_SIDE    2

//The LED grid is 32x12. The Pong animation grid size is 30x10 because we have
//a 1-pixel border around the grid. Therefore we lose 2 pixels on the x,y planes.
#define GRID_X        30
#define GRID_Y        10

//The radius of the starting ball
#define BALL_RAD      1

//The amount of balls to start out on the grid
#define BALL_AMOUNT   7

//The speed that the balls bounce around at
#define GAME_SPEED    45

/*************************************************
*                   Macros                       *
*************************************************/

/*************************************************
*              Function Prototypes               *
*************************************************/ 
void Invert_Grid(void);
void Draw_Border(UINT8 width); 
void Reset_Sequences(UINT8 mode);
void Scoreboard(UINT32 sensor_bits);
void Corner_Circles_On_Detection(void);
void Scrolling_Arrows_On_Detection(void);
void Ball_Washers_Detect(UINT8 detection);  
void Check_Pod_Animation(UINT8 *selection);
void Check_Grid_Animation(UINT8 *selection);
void Check_Ring_Animation(UINT8 *selection);
void LED_Pixel(UINT8 px, UINT8 py, UINT8 state);
void Set_Text(UINT8 px, UINT8 py, char text[12]);
void Draw_Circle(UINT8 px, UINT8 py, UINT8 radius);
void Draw_Rect(UINT8 px, UINT8 py, UINT8 sx, UINT8 sy);  
void Pod_Detect(UINT32 detection, RGB off_color, RGB on_color);
void Ball_Washer(UINT8 bw, UINT16 fan_speed, UINT16 pump_speed);

UINT8 Intro_Animation(void);

INT8 On_Pod_Change(UINT32 sensor_bits);  

UINT8 Checkers(void);  
UINT8 Ring_Chase(void);
UINT8 Circle_Out(void); 
UINT8 Cycle_Rings(void);  
UINT8 Box_Grid_In(void);  
UINT8 Update_Text(void);  
UINT8 Cycle_Colors(void);
UINT8 Pyramid_Chase(void);
UINT8 Pong_Animation(void);
UINT8 Corner_Circles(void);
UINT8 Fade_Pod_Colors(void);    
UINT8 End_Blast(UINT8 side);
UINT8 Crossfade_Rings(void); 
UINT8 Exploding_Circle(void); 
UINT8 Draw_Sine(UINT8 state);  
UINT8 Dual_Wave(UINT8 state);
UINT8 Cycle_Pod_Animations(void);
UINT8 Cycle_Grid_Animations(void);
UINT8 Cycle_Ring_Animations(void);
UINT8 Scrolling_Arrows(UINT8 direction);
UINT8 Ripple_Out(UINT16 fade_rate, UINT16 delay);
UINT8 Set_Scrolling_Text(char text[64]);
UINT8 Color_Throb(RGB outside_color, RGB inside_color);    


UINT8 Cycle_Pod_Animations_Sense(void);        

#endif
