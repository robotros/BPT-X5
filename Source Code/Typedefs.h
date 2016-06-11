/*******************************************************************************
* Title:                                                                       *
* Version: 1.0                                                                 *
* Author: Jeff Nybo                                                            *
* Date:                                                                        *
*                                                                              *
* Description:                                                                 *
*                                                                              *
*******************************************************************************/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

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

#define _FAR     __attribute__((far))

/*************************************************
*                  Constants                     *
*************************************************/ 
#define FOSC  140000000LL
#define FCYC  (FOSC / 2)

#define TOSC  (1 / FOSC)
#define TCYC  (2 / FOSC)

#define HIGH  0x01
#define LOW   0x00

#define TRUE  0x01
#define FALSE 0x00

#define ON    0x01
#define OFF   0x00

//Defines the array size of the seq[x] variables
#define SEQ_AMOUNT		50

//Defines the buffer size for SD card operation (Must be multiple of 512)
#define SD_BUF_SIZE   4096

//The amount of deafult colors
#define COLOR_COUNT         11

//The current version of code for the BPT X5
#define BPT_X5_VERSION		  "1.0"

//The 11 color defaults
#define BLACK               0  
#define RED                 1 
#define GREEN               2
#define BLUE                3
#define CYAN                4
#define MAGENTA             5
#define YELLOW              6
#define PINK                7
#define ORANGE              8
#define VIOLET              9
#define WHITE               10

/*************************************************
*                   Macros                       *
*************************************************/
#define BitSet(var,bitno) (var|=(1<<bitno))
#define BitClr(var,bitno) (var&=~(1<<bitno))
#define BitTst(var,bitno) (var&(1<<bitno))

#define COMBINE16(MSB,LSB)      (((UINT16)MSB << 8) | LSB)
#define COMBINE32(B3,B2,B1,B0)  (((((UINT32)B3 << 24) | (UINT32)B2 << 16) | \
                                    (UINT32)B1 << 8) | B0)                                    

/*************************************************
*               Global Variables                 *
*************************************************/
typedef struct
{
  float red;
  float green;
  float blue;
  UINT16 delay;
} RGB_FADE; 

typedef struct
{
  float red;
  float green;
  float blue;
} RGB; 

typedef struct
{
  float value;
  UINT16 delay;
} RING_FADE;

typedef struct
{
    UINT8  FAT_copies;
    UINT8  sectors_cluster;
    UINT8  partition_start;
    
    UINT16 signature;
    UINT16 sector_size;
    UINT16 reserved_sectors;
    
    UINT32 card_capacity;
    UINT32 start_FAT;
    UINT32 root_start;
    UINT32 sectors_FAT;
    UINT32 cluster_size;
    UINT32 total_sectors;
    UINT32 total_clusters;
    UINT32 root_dir_cluster;
} FILE_SYSTEM; 

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
