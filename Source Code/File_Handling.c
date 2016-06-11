/*******************************************************************************
* Title: File_Handling.c                                                         
* Version: 1.0                                                                   
* Author: Jeff Nybo                                                              
* Date: March 13, 2015                                                    
*                                                                                
* Description:                                                                 
*                                                                               
*                                                                               
*******************************************************************************/

#ifndef FILE_HANDLING_C
#define FILE_HANDLING_C

#include "Main_Includes.h"
#include "File_Handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FAT32_Setup.h"
#include "SD_Setup.h"
#include "Delay_Setup.h"

extern UINT8 _FAR SD_buf[SD_BUF_SIZE];

extern volatile T16_FLAG FLAG1;

extern volatile FILE_SYSTEM FAT32;

/*******************************************************************************
* Function:                                                                      
*                                                                               
* Variables:                                                                    
* N/A                                                                           
*                                                                               
* Description:                                                                  
*                                                                               
*******************************************************************************/
void Run_SD_Animation(char filename[16])
{ 
  UINT16 code;
  UINT32 capacity;
  SD_FILE FILE1;
	 
  code = FAT32_Open_File(filename,&FILE1);
  FAT32_File_Properties(&FILE1);
  
  if (code)
    printf("Error Opening File: %d\r\n\r\n",code);
    
  else
    printf("Successful File Open: %d\r\n\r\n",code);
    
  capacity = FAT32_Last_Cluster(FILE1.start_cluster);
  printf("Last Cluster: %lu\r\n\r\n",capacity);
    
  capacity = FAT32_Get_Offset(FILE1.size,FAT32.cluster_size);
  printf("Offset: %lu\r\n\r\n",capacity);  
  
  memset(SD_buf,0x00,4096);
  
  SD_Read_Mul_Sectors(FILE1.start_sector,8);
  
  printf("\r\n\r\n");
  FAT32_Display_Data(SD_buf,4096,1);
  printf("\r\n\r\n");
}
  
#endif
