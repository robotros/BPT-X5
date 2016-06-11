/*******************************************************************************
* Title: FAT32_Setup.h                                                         
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                      
*                                                                              
* Description:                                                                 
* This file contains the constants and function prototypes that are used for    
* initializing and controlling an SD Card formatted as FAT32.                                  
*******************************************************************************/

#ifndef FAT32_SETUP_H
#define FAT32_SETUP_H

/*************************************************
*               Bit Definitions                  *
*************************************************/
#define FAT32_PAUSE            FLAG1.b3

/*************************************************
*                  Constants                     *
*************************************************/ 
//Error Messages
#define FAT32_SUCCESS         			0
#define ERROR_FORMAT          			1
#define ERROR_SIGNATURE       			2
			
#define ERROR_LFN             			0x10
			
#define READ_IN_PROGRESS      			0x11
			
#define FAT32_CARD_FULL       			0x00
			
#define FAT32_FILE_NOT_FOUND  			0x13
			
#define FAT32_EOF                  	0x0FFFFFFF

/*************************************************
*                   Macros                       *
*************************************************/

typedef struct
{
    char   name[13]; 
    UINT8  entry;
    UINT32 start_cluster;
    UINT32 last_cluster;
    UINT32 total_clusters;
    UINT32 total_sectors;
    UINT32 start_sector;
    UINT32 size;
    
} SD_FILE;  

/*************************************************
*              Function Prototypes               *
*************************************************/ 
void FAT32_Display_MBR(FILE_SYSTEM *FS);
void FAT32_File_Properties(SD_FILE *FILE1);
void FAT32_Display_Data(UINT8 *buf, UINT16 len, UINT8 format);

UINT8 FAT32_Init(void); 
UINT8 FAT32_Read_File(SD_FILE *FILE1);
UINT8 FAT32_Write_FAT(UINT32 cluster, UINT32 data);
UINT8 FAT32_Open_File(char file_name[13], SD_FILE *FILE1); 
UINT8 FAT32_Write_File(char filename[13],UINT32 filesize);

UINT16 FAT32_Get_Offset(UINT32 filesize, UINT16 buf_size);

UINT32 FAT32_Next_Free_Cluster(void);
UINT32 FAT32_Total_Free_Clusters(void);
UINT32 FAT32_Next_Cluster(UINT32 cluster);

#endif
