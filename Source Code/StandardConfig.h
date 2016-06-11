/*******************************************************************************
* Title: StandardConfig.h                                                      *
* Version: 1.0                                                                 *
* Author: Jeff Nybo                                                            *
* Date: October 15, 2010                                                       *
*                                                                              *
* Description:                                                                 *
* This source file contains the configuration bits that will initialize the    *
* clock settings, code protection, etc. It also contains a few Quick Keywords  *
* to shorten the text length when declaring some variables.                    *
*******************************************************************************/

#ifndef STANDARDCONFIG_H
#define STANDARDCONFIG_H

/*************************************************
*             Configuration Settings             *
*************************************************/
//_CONFIG1(0x3F7F); 
//_CONFIG2(0xFBFE);

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & 
         BKBUG_OFF & COE_OFF & ICS_PGx1 &  
         WINDIS_OFF & FWPSA_PR32 & WDTPS_PS32768);
         
_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & 
         FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_ON &
         IOL1WAY_ON & I2C1SEL_PRI & POSCMOD_HS);

/*************************************************
*                   EEPROM                       *
*************************************************/

/*************************************************
*                 Quick Keywords                 *
*************************************************/
typedef unsigned long int uint32;
typedef signed long int int32;

typedef unsigned int uint16;
typedef signed int int16;

typedef unsigned char uint8;
typedef unsigned char byte;
typedef signed char int8;

typedef unsigned long int UINT32;
typedef unsigned long int DWORD;
typedef signed long int INT32;

typedef unsigned int UINT16;
typedef unsigned int WORD;
typedef signed int INT16;

typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef signed char INT8;

/*************************************************
*                  Constants                     *
*************************************************/ 
#define FOSC  32000000LL
#define FCYC  (FOSC / 2)

#define TOSC  (1 / FOSC)
#define TCYC  (2 / FOSC)

#define HIGH  0x01
#define LOW   0x00

#define TRUE  0x01
#define FALSE 0x00

#define ON    0x01
#define OFF   0x00

/*************************************************
*                   Macros                       *
*************************************************/
#define BitSet(var,bitno) (var|=(1<<bitno))
#define BitClr(var,bitno) (var&=~(1<<bitno))
#define BitTst(var,bitno) (var&(1<<bitno))

/*************************************************
*               Global Variables                 *
*************************************************/
extern volatile uint16 count;

uint32 prg_cnt32 = 0;

typedef struct
{
  float red;
  float green;
  float blue;
  uint16 delay;
} DATA2; 

typedef struct
{
  float red;
  float green;
  float blue;
} RGB; 

RGB LEDn[20];
DATA2 DIFF[20];

RGB *pLEDn = &LEDn[0];
DATA2 *pDIFF = &DIFF[0];

uint8 RGB_data2[96];

uint8 seq[2];
uint8 start[5] = {5,10,14,19,23};

//Bit Variables
typedef union tag8bits{
   struct {
     uint8 all;
   };
   struct {  
      unsigned b0:1;    
      unsigned b1:1;    
      unsigned b2:1;    
      unsigned b3:1;
      unsigned b4:1;  
      unsigned b5:1;    
      unsigned b6:1;
      unsigned b7:1;            
   };
 }T8_FLAG;

typedef union tag16bits{
   struct {
     uint16 all;
   };
   struct {  
      unsigned b0:1;    
      unsigned b1:1;    
      unsigned b2:1;    
      unsigned b3:1;
      unsigned b4:1;  
      unsigned b5:1;    
      unsigned b6:1;
      unsigned b7:1;
      unsigned b8:1;    
      unsigned b9:1;    
      unsigned b10:1;    
      unsigned b11:1;
      unsigned b12:1;  
      unsigned b13:1;    
      unsigned b14:1;
      unsigned b15:1;              
   };
 }T16_FLAG;

typedef union tag32bits{
   struct {
     uint32 all;
   };
   struct {  
      unsigned b0:1;    
      unsigned b1:1;    
      unsigned b2:1;    
      unsigned b3:1;
      unsigned b4:1;  
      unsigned b5:1;    
      unsigned b6:1;
      unsigned b7:1;
      unsigned b8:1;    
      unsigned b9:1;    
      unsigned b10:1;    
      unsigned b11:1;
      unsigned b12:1;  
      unsigned b13:1;    
      unsigned b14:1;
      unsigned b15:1;   
      unsigned b16:1;    
      unsigned b17:1;    
      unsigned b18:1;    
      unsigned b19:1;    
      unsigned b20:1;    
      unsigned b21:1;    
      unsigned b22:1;    
      unsigned b23:1;    
      unsigned b24:1;    
      unsigned b25:1;    
      unsigned b26:1;    
      unsigned b27:1;    
      unsigned b28:1;    
      unsigned b29:1;    
      unsigned b30:1;    
      unsigned b31:1;             
   };
 }T32_FLAG;

#endif
