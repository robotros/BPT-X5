/*******************************************************************************
* Title: FAT32_Setup.c                                                         
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file contains the functions that are used to initialize and control the 
* operation of an MMC/SD Card.                                                 
*******************************************************************************/  
    
#ifndef FAT32_SETUP_C
#define FAT32_SETUP_C

#include "Main_Includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FAT32_Setup.h"
#include "SD_Setup.h"
#include "Delay_Setup.h"

/*************************************************
*               Global Variables                 *
*************************************************/ 
extern UINT8 _FAR SD_buf[SD_BUF_SIZE];
extern volatile FILE_SYSTEM FAT32;
extern volatile T16_FLAG FLAG1;

/*******************************************************************************
* Function: FAT32_Init(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will read the MBR of the SD card and determine whether it is                                                                              
* formatted with the FAT32 file format or not.                                                                             
*******************************************************************************/
UINT8 FAT32_Init(void)
{ 
  UINT8 response;
  char format[6] = "FAT32";
  
  //Start sector of the partition
  FAT32.partition_start = 0;
  
  //Read the MBR
  response = SD_Read_Sector(0,SD_buf);
  
  //If there was a read error, return error
  if (response)
    return response; 
  
  //Check to see if it is formatted as FAT32; If not, return error
  if (strncmp((const char*)&SD_buf[82],format,5) != 0)
    return ERROR_FORMAT;

  //Read the MBR and store the correct data to navigate the card
  FAT32.FAT_copies = SD_buf[16];
  FAT32.sectors_cluster = SD_buf[13];
  FAT32.sector_size = COMBINE16(SD_buf[12],SD_buf[11]);
  FAT32.reserved_sectors = COMBINE16(SD_buf[15],SD_buf[14]);
  FAT32.signature = COMBINE16(SD_buf[511],SD_buf[510]);
  
  //If there is no signature at the end of the MBR, return an error
  if (FAT32.signature != 0xAA55)
    return ERROR_SIGNATURE;
  
  /////////////////////////////////////////////
  // CHECK PARTITION TYPE HERE IF APPLICABLE //
  /////////////////////////////////////////////
  
  //Get the root directory cluster
  FAT32.root_dir_cluster = COMBINE32(SD_buf[47],SD_buf[46],SD_buf[45],SD_buf[44]);
  
  //Find the amount of sectors deicated to each FAT
  FAT32.sectors_FAT = COMBINE32(SD_buf[39],SD_buf[38],SD_buf[37],SD_buf[36]);
  
  //Find the starting location of the FAT
  FAT32.start_FAT = FAT32.reserved_sectors + FAT32.partition_start;
  
  //Calculate the root start
  FAT32.root_start = (FAT32.sectors_FAT * FAT32.FAT_copies) + FAT32.reserved_sectors + FAT32.partition_start;
  
  //Calculate the size of each cluster in bytes
  FAT32.cluster_size = FAT32.sectors_cluster * FAT32.sector_size;
  
  //Calculate the total amount of sectors on the SD card
  FAT32.total_sectors = SD_Capacity() / 512;
  
  //Calculate the total amount of write-able clusters on the SD card
  FAT32.total_clusters = FAT32.sectors_FAT * 128;
  
  //Calculate the total amount of clusters on the SD card
  FAT32.card_capacity = SD_Capacity();
  
  //Operation completed successfully
  return FAT32_SUCCESS;
} 

/*******************************************************************************
* Function: FAT32_Open_File(char filename[13], SD_FILE *FILE1)                                                                   
*                                                                              
* Variables:                                                                   
* filename -> Contains the name of the file to be opened
* *FILE1   -> The structure where the main file information will be stored                                                                         
*                                                                              
* Description:                                                                 
* This function will take a filename and search the directory entry for it. If it                                                                             
* doesn't find the file, it will return an error. If it does, it will parse the
* directory entry and store various information on how to access the file data and
* clusters. If successful, it will return a 0;                                                                             
*******************************************************************************/
UINT8 FAT32_Open_File(char filename[13], SD_FILE *FILE1)
{  
	UINT8 response;
	UINT8 quit = 0;
	UINT16 i = 0;
	UINT16 j = 0;
	UINT16 k = 0;
	UINT16 loc;
	
	//Clear all of the old data stored in the SD buffer
	memset(SD_buf,0x00,SD_SECTOR_SIZE);
	
	//Set the start sector of the card (always 0 for us; Only 1 Partition)
	FILE1->start_sector = 0;  
	
	//Clear unknown data in name variable
	for (i = 0;i< 11;i++)
	  FILE1->name[i] = ' '; 
	
	//Reset counter
	i = 0;
	
	//Copy the filename into the struct
	do
	{
	  FILE1->name[i] = filename[i];
	  i++;
	} while (filename[i] != '.' && i < 8);
	
	//Update the counter address
	i++;
	
	//Save the file extension
	FILE1->name[8] = filename[i];
	FILE1->name[9] = filename[i+1];
	FILE1->name[10] = filename[i+2];
	FILE1->name[11] = '\0';  
	
	//Read the first sector of the root start
	response = SD_Read_Sector(FAT32.root_start,SD_buf);
	 
	//If there was a read error, return error
	if (response)
	  return response; 
	 
	//Reset Counters
	i = 0;
	k = 0;
	
	//Begin sifting through the directory entries and look for the file
	do
	{
	  //Each directory entry is 32 bytes long; Calculate next entry location
	  loc = i * 32;
	  
	  //Update the file counter   
	  k++;   
	  
	  //If the file has been found, store all of the data in the struct
	  if (strncmp(FILE1->name,(const char*)&SD_buf[loc],11) == 0)
	  {
	    //Save the directory entry number
	    FILE1->entry = k + (16 * j);
	   
	    //Find the size of the file in bytes
	    FILE1->size = COMBINE32(SD_buf[31+loc],SD_buf[30+loc],SD_buf[29+loc],SD_buf[28+loc]);
	   
	    //Locate the start cluster of the file
	    FILE1->start_cluster = COMBINE32(SD_buf[21+loc],SD_buf[20+loc],SD_buf[27+loc],SD_buf[26+loc]); 
	   
	    //Locate the start sector of the file
	    FILE1->start_sector = ((FILE1->start_cluster - 2) * FAT32.sectors_cluster) + FAT32.root_start;
	   
	    //Calculate the total amount of sectors used by the file
	    FILE1->total_sectors = FILE1->size /FAT32.sector_size;
	   
	    //If the last data of the file is included in another sector, update total sectors
	    if ((FILE1->size % FAT32.sector_size) != 0)
	      FILE1->total_sectors++;    
	   
	    //Calculate the total amount of clusters used by the file
	    FILE1->total_clusters = FILE1->total_sectors / FAT32.sectors_cluster;
	    
	    //If the last data of the file is included in the next cluster, update total clusters
	    if ((FILE1->total_sectors % FAT32.sectors_cluster) != 0)
	      FILE1->total_clusters++;
	    
	    //Break out of the loop
	    quit = 1;
	    break;
	  } 
	  
	  //Update the directory entry counter
	  i++;   
	  
	  //Check to see if we are at the end of the current root directory sector
	  if (loc >= 480)
	  {
	    //Read the next root directory sector
	    j++;
	    response = SD_Read_Sector(FAT32.root_start + j,SD_buf);
	 
	     //If there was a read error, return error
	     if (response)
	       return response; 
	    
	    //Update the 32-byte directory entry start location
	    loc = 0;
	    
	    //Reset the directory entry counter
	    i = 0;
	  }  
	} while (quit == 0 && SD_buf[loc] != 0x00);  
	
	//If file was not found, return an error
	if (SD_buf[loc] == 0x00)
	  return FAT32_FILE_NOT_FOUND;  
	
	//File was found, return success
	return FAT32_SUCCESS;
}  

/*******************************************************************************
* Function: FAT32_Read_File(SD_FILE *FILE1)                                                                   
*                                                                              
* Variables:                                                                   
* *FILE1 -> Contains the file structure information of the file to be read                                                                         
*                                                                              
* Description:                                                                 
* This function will read all of the data from the SD_FILE structure that is passed                                                                             
* into it. Due to the RAM limitations, once the buffer has filled up with data,
* the function will save its current location in the data and return a 
* READ_IN_PROGRESS value. This will allow the user to parse the received buffer data
* and then run the function again, doing this until a FAT32_SUCCESS value is returned,
* indicating that all of the file has been read. The buffer size must be in multiples
* of 512 bytes, as whole sectors are read into the buffer.                                                                             
*******************************************************************************/
UINT8 FAT32_Read_File(SD_FILE *FILE1)
{
  static UINT32 last_sector;
  static UINT32 last_cluster;
  static UINT32 last_file_left;
  static UINT32 last_cluster_left;
  
  UINT32 next_sector;
  UINT32 next_cluster;
  UINT32 file_left;
  
  UINT8 response;
  UINT8 cluster_left;
  UINT16 bc = 0;
  
  //Function was paused so that user could unload full buffer
  //Reload last values and continue reading the file, filling up the buffer again
  if (FAT32_PAUSE)
  {
    //Reload sector address where we left off last
    next_sector = last_sector;
    
    //Reload the location of the cluster from where we left off
    next_cluster = last_cluster;
    
    //Reload the remaining file data that has to be read
    file_left = last_file_left;
    
    //Reload the amount of sectors that need to be read in the current cluster
    cluster_left = last_cluster_left;
  }
  
  //All previous operations have been completed, start from beginning of file  
  else
  {
    //Starting location of first cluster
    next_sector = FILE1->start_sector;
    
    //Starting location of first cluster
    next_cluster = FILE1->start_cluster;
    
    //Keeps track of how many sectors are left to read in the cluster
    cluster_left = FAT32.sectors_cluster;
    
    //Keeps track of the file data that is left to read
    file_left = FILE1->size;
  }  
  
  //Begin reading every sector/cluster of data until the EOF is reached  
  do
  { 
    //Save the location of the last cluster that contains data (When loop is finished,
    //this variable will contain the last cluster location
    FILE1->last_cluster = next_cluster;
      
    //If all of the data has been read, return a successful read
    if (file_left == 0)
    {
      //Clear pause bit
      FAT32_PAUSE = 0;
      
      //End of file; Return success
      return FAT32_SUCCESS;
    }  
    
    //If the file data is less than or equal to a full sector,
    //read the sector and return a successful read
    else if (file_left <= FAT32.sector_size)
    {
      //Read last sector of data
      response = SD_Read_Sector(next_sector,&SD_buf[bc]);
          
      //If there was a read error, return error
      if (response)
        return response; 
      
      //Clear pause bit
      FAT32_PAUSE = 0;
      
      //End of file; Return succesful read
      return FAT32_SUCCESS;
    }
    
    //Check if we have read all of the sectors in the cluster
    if (cluster_left == 0x00)
    {
      //Get the next cluster location
      next_cluster = FAT32_Next_Cluster(next_cluster);
      
      //If the End of File has been reached, return a successful read
      if (next_cluster >= 0x0FFFFFF8)
      {
        //Clear pause bit
        FAT32_PAUSE = 0;
        
        //End of file; Return success
        return FAT32_SUCCESS;
      }  
      
      //Calculate the next sector that needs to be read    
      next_sector = ((next_cluster - 2) * FAT32.sectors_cluster ) + FAT32.root_start;
      
      //Reset the cluster counter so that all the sectors in the new cluster will be read
      cluster_left = FAT32.sectors_cluster;
    }
  
    //Read the next sector of data 
    response = SD_Read_Sector(next_sector,&SD_buf[bc]);
     
    //If there was a read error, return error
    if (response)
      return response; 
    
    //Update the buffer location
    bc += FAT32.sector_size;  
    
    //Decrease the file size by the amount of data that was just read
    file_left -= FAT32.sector_size;
    
    //Update the next sector address and the amount of sectors still left to read
    next_sector++;
    cluster_left--;
    
    //Check to see if our buffer has filled up; If it has, set a flag and pause reading
    if (bc >= SD_BUF_SIZE)
    {
      //Pause reading here; set a flag to use the received data before overwriting it
      FAT32_PAUSE = 1;
      
      //Save the current values for the next loop through after re-buffering data
      last_sector = next_sector;
      last_cluster = next_cluster;
      last_file_left = file_left;
      last_cluster_left = cluster_left;
      
      //Return read in progress
      return READ_IN_PROGRESS;
    }
  }
  while (file_left > 0);  

  //Clear pause bit
  FAT32_PAUSE = 0;

  //Return success
  return FAT32_SUCCESS;
}

/*******************************************************************************
* Function: FAT32_Write_File(char filename[13],UINT32 filesize)                                                                   
*                                                                              
* Variables:                                                                   
* filename -> The name of the file to be created (Must be < 12 chars w/ extension)                                                                         
* size -> The size of the file to be created
*                                                                             
* Description:                                                                 
* This function will create a file with the specified filename as long as it is
* not already taken.                                                                              
*                                                                              
*******************************************************************************/
UINT8 FAT32_Write_File(char filename[13],UINT32 filesize)
{
  UINT8 response;
  UINT8 free = 0;
  UINT8 sectors_read = 0;
  
  UINT16 j;
  UINT16 entry;
  
  UINT32 i;
  UINT32 loc;
  UINT32 cluster;
  UINT32 next_cluster;
  UINT32 last_cluster;
  UINT32 start_cluster;
  UINT32 clusters_needed;
  UINT32 clusters_left;
  
  //Get the amount of free clusters left on the SD card
  clusters_left = FAT32_Total_Free_Clusters();
  
  //Calculate the amount of clusters needed to store this files data
  clusters_needed = filesize / (FAT32.sectors_cluster * FAT32.sector_size);
  
  //If the last data of the file is included in the next cluster, update needed clusters
  if ((filesize % (FAT32.sectors_cluster * FAT32.sector_size)) != 0)
    clusters_needed++;
  
  //If we do not have enough room on the SD card, return an error  
  if (clusters_needed > clusters_left)
    return FAT32_CARD_FULL; 
 
  //Set up the first free cluster entry for the data
  start_cluster = FAT32_Next_Free_Cluster();       
  
  //These next two lines ready the loop below to search for more free clusters if needed
  last_cluster = start_cluster;
  FAT32_Write_FAT(start_cluster,0x0FFFFFFF);
  
  //Assign a cluster entry to each cluster of data that is needed for this file 
  for (i = 1;i < clusters_needed;i++)
  { 
    //Calculate the entry location of the next free cluster that is available  
    next_cluster = FAT32_Next_Free_Cluster();
    
    //Update the last FAT cluster entry to point to the next cluster for the data
    response = FAT32_Write_FAT(last_cluster,next_cluster);    
     
    //If there was a write error, return error
    if (response)
      return response; 
    
    //Write dummy data into the newly found cluster so that next_cluster on the next
    //loop is not assigned to this unused cluster
    response = FAT32_Write_FAT(next_cluster,FAT32_EOF);  
     
    //If there was a write error, return error
    if (response)
      return response; 

    ///////////////////////////////////////////////////////////////////////////////////////
    // Add code here to parse buffered data for the new file
    // We may not have enough RAM to buffer all of the file data,
    // write a routine to parse the data into 512 byte sectors and
    // write it to the current cluster.
    //
    //Ex)
    //SD_Write_Mul_Sectors(((last_cluster-2)*8) + FAT32.root_start,FAT32.sectors_cluster);
    ////////////////////////////////////////////////////////////////////////////////////////
    
    SD_Write_Mul_Sectors(((last_cluster-2)*8) + FAT32.root_start,FAT32.sectors_cluster);
    // SD_Write_Sector(((last_cluster-2)*8) + FAT32.root_start,SD_buf);
   
    //Store the previously found cluster for the next loop
    last_cluster = next_cluster;  
  }
      
  ///////////////////////////////////////////////////////////////////////////////////////
  // Add code here to parse the last cluster of buffered data for the new file
  // We may not have enough RAM to buffer all of the file data,
  // write a routine to parse the data into 512 byte sectors and
  // write it to the last cluster.
  //
  //Ex)
  //SD_Write_Mul_Sectors(((last_cluster-2)*8) + FAT32.root_start,FAT32.sectors_cluster);
  ////////////////////////////////////////////////////////////////////////////////////////
  
  SD_Write_Mul_Sectors(((last_cluster-2)*8) + FAT32.root_start,FAT32.sectors_cluster);
  //SD_Write_Sector(((last_cluster-2)*8) + FAT32.root_start,SD_buf);
  
  //The first sector of the directory entry is the root start (In the loop
  //below, loc++ is incremented at the beginning of the loop, hence the - 1)
  loc = FAT32.root_start - 1;
  
  //Cluster #2 contains the first sector(s) of the directory entry
  cluster = 2;
  
  //Keep looping until a free directory entry is found
  while (free == 0)
  {  
    //Read the next sector in the cluster
    loc++; 
    
    //Check if all sectors of the directory entry cluster have been parsed
    if (sectors_read == FAT32.sectors_cluster)
    {
      //Store the last cluster location
      last_cluster = cluster;
      
      //Get the cluster location of the next directory entry
      cluster = FAT32_Next_Cluster(cluster);
      
      //Calculate the starting sector location of the next cluster  
      loc = ((cluster - 2) * FAT32.sectors_cluster) + FAT32.root_start;
      
      //Check if we need to allocate a new cluster for more directory entries
      if (cluster == FAT32_EOF)
      {
        //Find the next free cluster
        cluster = FAT32_Next_Free_Cluster();
        
        //Calculate the new starting sector location of the next cluster  
        loc = ((cluster - 2) * FAT32.sectors_cluster) + FAT32.root_start;
        
        //Clear all byte entries in cluster to 0x00 for directory entries
        //The erase function would work more optimal here, however, I found
        //that some cheaper SD cards will not actually erase the sector even
        //though it says it does. 
        //SD_Erase_Sectors(loc,FAT32.sectors_cluster);
        memset(SD_buf,0x00,512);
        SD_Write_Mul_Sectors(loc,FAT32.sectors_cluster);
  
        //Update the FAT with the new cluster location
        response = FAT32_Write_FAT(last_cluster,cluster);
     
        //If there was a write error, return error
        if (response)
          return response; 
          
        response = FAT32_Write_FAT(cluster,FAT32_EOF);  
         
        //If there was a write error, return error
        if (response)
          return response; 
      }  
      
      //Reset the sector read count to 0
      sectors_read = 0;
    }
    
    //Read the specified sector filled with directory entries
    response = SD_Read_Sector(loc,SD_buf);
     
    //If there was a read error, return error
    if (response)
      return response; 
    
    //Increment the sector read counter
    sectors_read++;     
    
    //Loop through all 16 possible entries in the sector
    for (j = 0;j < 16;j++)
    {
      //Calculate the byte offset of the directory entry to be searched
      entry = j * 32;
      
      //Check if there are any unallocated or erased directory entries
      if ((SD_buf[entry] == 0x00) || (SD_buf[entry] == 0xE5))
      {
        //An unused entry space has been found, quit searching
        free = 1;
        j = 17;
      }
    }  
  }    
  
  //Erase all data in the free directory entry to prepare for a new write
  memset(&SD_buf[entry],0x20,32);
  
  //Copy the filename into the directory entry (Up to 8 characters not including extension) 
  for (i = 0;i < 8;i++)
  { 
    //If the filename has been copied and is less than 8 characters, break from loop
    if (filename[i] == '.')
      break;
      
    //Copy filename into entry location in buffer
    SD_buf[entry+i] = filename[i];
  }  
  
  //Copy the file extension into the directory
  SD_buf[entry+8] = filename[i+1];
  SD_buf[entry+9] = filename[i+2];
  SD_buf[entry+10] = filename[i+3];
  
  //Copy the location of the first cluster into entry location in buffer
  SD_buf[entry+21] =  start_cluster >> 24;
  SD_buf[entry+20] = (start_cluster & 0x00FF0000) >> 16;
  SD_buf[entry+27] = (start_cluster & 0x0000FF00) >> 8;
  SD_buf[entry+26] =  start_cluster & 0x000000FF;
  
  //Copy the filesize of the file into the entry location in the buffer
  SD_buf[entry+31] =  filesize >> 24;
  SD_buf[entry+30] = (filesize & 0x00FF0000) >> 16;
  SD_buf[entry+29] = (filesize & 0x0000FF00) >> 8;
  SD_buf[entry+28] =  filesize & 0x000000FF;
  
  //These values can be modified; An RTC is needed to have an accurate time of the
  //file creation. By default, the timestamp will be set to September 4, 2014, 10:51:10PM
  SD_buf[entry+11] = 0x00;
  SD_buf[entry+12] = 0x18;
  SD_buf[entry+13] = 0x19;
  SD_buf[entry+14] = 0x65;
  SD_buf[entry+15] = 0xB6;
  SD_buf[entry+16] = 0x24;
  SD_buf[entry+17] = 0x45;
  SD_buf[entry+18] = 0x24;
  SD_buf[entry+19] = 0x45;
  SD_buf[entry+22] = 0x66;
  SD_buf[entry+23] = 0xB6;
  SD_buf[entry+24] = 0x24;
  SD_buf[entry+25] = 0x45;
  
  //Update the directory entry sector with the new file information
  response = SD_Write_Sector(loc,SD_buf);
     
  //If there was a write error, return error
  if (response)
    return response; 

  //Return successfule file creation
  return FAT32_SUCCESS; 
}
  
/*******************************************************************************
* Function: FAT32_Total_Free_Clusters(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will get the total amount of free clusters that are available to                                                                            
* write and return the amount.                                                                              
*******************************************************************************/
UINT32 FAT32_Total_Free_Clusters(void)
{
  UINT8 buf[512];
  UINT8 response;
  UINT32 free_clusters;
  
  //Read the boot sector which contains the free cluster amount
  response = SD_Read_Sector(FAT32.partition_start+1,buf);
     
  //If there was a read error, return 0 clusters available
  if (response)
    return 0x00000000; 
  
  //Get the amount of free clusters
  free_clusters = COMBINE32(buf[491],buf[490],buf[489],buf[488]);
  
  //Return the total amount of free clusters
  return free_clusters;
}  
  
/*******************************************************************************
* Function: FAT32_Next_Cluster(UINT32 cluster)                                                                   
*                                                                              
* Variables:                                                                   
* cluster -> The current cluster location of the file being written/read                                                                         
*                                                                              
* Description:                                                                 
* This function will determine the next cluster in the files cluster chain                                                                              
*******************************************************************************/
UINT32 FAT32_Next_Cluster(UINT32 cluster)
{
  UINT8 buf[512];
  UINT8 response;
  UINT32 next_cluster;
  UINT32 FAT_sector;
  UINT8 el;
  
  //Get sector location that conatins the next cluster info
  FAT_sector = (cluster & 0x0FFFFF80) >> 7;
  
  //Get the entry location for the next cluster; 4-Bytes to each entry
  el = (cluster & 0x7F) * 4;
  
  //Read the sector of the FAT that contain the cluster entry
  response = SD_Read_Sector(FAT32.start_FAT + FAT_sector,buf);
     
  //Read Error; Return 0 which is a reserved cluster, so we know its an error
  if (response)
    return 0x00000000; 
  
  //Store the next cluster data
  next_cluster = COMBINE32(buf[el+3],buf[el+2],buf[el+1],buf[el]);
  
  //return the next cluster
  return next_cluster;
}
  
/*******************************************************************************
* Function: FAT32_Last_Cluster(UINT32 cluster)                                                                   
*                                                                              
* Variables:                                                                   
* cluster -> The start cluster location of the file being written/read                                                                         
*                                                                              
* Description:                                                                 
* This function will determine the last cluster in the files cluster chain                                                                              
*******************************************************************************/
UINT32 FAT32_Last_Cluster(UINT32 cluster)
{
  UINT8 buf[512];
  UINT8 response;
  UINT32 last_cluster;
  UINT32 FAT_sector;
  UINT8 el;
  
  //Loop until EOF has been found
  while (cluster != FAT32_EOF)
  {
    //Get sector location that conatins the next cluster info
    FAT_sector = (cluster & 0x0FFFFF80) >> 7;
    
    //Get the entry location for the next cluster; 4-Bytes to each entry
    el = (cluster & 0x7F) * 4;
    
    //Read the sector of the FAT that contain the cluster entry
    response = SD_Read_Sector(FAT32.start_FAT + FAT_sector,buf);
       
    //Read Error; Return 0 which is a reserved cluster, so we know its an error
    if (response)
      return 0x00000000; 
      
    last_cluster = cluster;
    
    //Store the next cluster data
    cluster = COMBINE32(buf[el+3],buf[el+2],buf[el+1],buf[el]);
  }  
  
  //return the next cluster
  return last_cluster;
}

/*******************************************************************************
* Function: FAT32_Get_Offset(UINT32 filesize, UINT16 buf_size)                                                                   
*                                                                              
* Variables:                                                                   
* filesize -> The size of the file in bytes                                                                         
* buf_size -> The size of the data buffer 
*                                                                             
* Description:                                                                 
* This function will return the amount of data that is valid in the last buffer 
* frame of data for the file.
*
* Example
* We have a file that is 9823 bytes in size and are using a buffer of 4096 bytes. 
* (9823 bytes / 4096 bytespersector) = 2.39819.., so in the last buffer read of 
* the data, not all of the bytes of data is used by the file. To find the amount of 
* valid data in the last buffer frame, we can run this function.
*
* offset = 9823 % 4096 (Buffer is 8 sectors large)
*        = 1631
*
* In my 4096 byte buffer, only the first 1631 bytes of the last buffer frame 
* pertain to the file. The rest of the 2465 bytes are unused and dummy bytes.                                                                                        
*******************************************************************************/
UINT16 FAT32_Get_Offset(UINT32 filesize, UINT16 buf_size)
{
  UINT16 offset;
  
  //Calculate the offset of the last of the buffered data
  offset = filesize % buf_size;
  
  //Return the offset
  return offset;
} 

/*******************************************************************************
* Function: FAT32_Next_Free_Cluster(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will search the File Allocation Table and return the next free
* cluster that is available. If no clusters are available, it will return a card                                                                             
* full error (0).                                                                              
*******************************************************************************/
UINT32 FAT32_Next_Free_Cluster(void)
{
  UINT8 buf[512];
  UINT16 loc = 0;
  UINT16 j;
  UINT32 i;
  UINT32 entry;
  
  //Read the starting sector of the FAT where we will begin looking for unused clusters
  SD_Read_Sector(FAT32.start_FAT,buf);
  
  //Loop until a cluster is found or the card is deemed full; Clusters 0 & 1 are reserved
  for (i = 2;i < FAT32.total_clusters;i++)
  {
    //Calculate the next cluster entry position to read
    j = (i % 128) * 4;
    
    //Check to see if we have reached the end of the sector
    if ((i % 128) == 0)
    {
      //Increment the sector location
      loc++;
      
      //Read the next sector in the FAT
      SD_Read_Sector(FAT32.start_FAT + loc,buf);
    }
    
    //Get the 32-bit cluster entry  
    entry = COMBINE32(buf[j+3],buf[j+2],buf[j+1],buf[j]);
    
    //If the cluster entry is equal to 0,that cluster is unused. Return unused cluster  
    if ((entry & 0x0FFFFFFF) == 0)
        return i;   
  }  
  
  //No free cluster was found, the card must be full          
  return FAT32_CARD_FULL;  
}  

/*******************************************************************************
* Function: FAT32_Write_FAT(UINT32 cluster, UINT32 data)                                                                    
*                                                                              
* Variables:                                                                   
* cluster -> The cluster that is to have data written into it
* data -> The data which should point to another cluster or indicate EOF                                                                        
*                                                                              
* Description:                                                                 
* This function will locate a cluster in the FAT and write the data vvariable                                                                             
* into it. The data variable should be the next cluster for the file or an EOF
* marker.                                                                              
*******************************************************************************/
UINT8 FAT32_Write_FAT(UINT32 cluster, UINT32 data)
{
  UINT8 buf[512];
  UINT8 response;
  
  UINT16 sector;
  UINT16 offset; 
  
  //Calculate the sector location of the cluster entry
  sector = (cluster >> 7) + FAT32.start_FAT;
  
  //Calculate the offset where the cluster entry begins in the sector
  offset = (cluster & 0x0000007F) * 4;
  
  //Read the whole sector
  response = SD_Read_Sector(sector,buf);
     
  //If there was a write error, return error
  if (response)
    return response; 
  
  //Modify the four bytes pertaining to the cluster entry
  buf[offset+3] = data >> 24;
  buf[offset+2] = (data & 0x00FF0000) >> 16;
  buf[offset+1] = (data & 0x0000FF00) >> 8;
  buf[offset] = data & 0x000000FF;
  
  //Write the modified values back into the sector along with the other stored data
  response = SD_Write_Sector(sector,buf);
     
  //If there was a write error, return error
  if (response)
    return response; 
    
  return FAT32_SUCCESS;
}  

/*******************************************************************************
* Function: FAT32_Display_MBR(FILE_SYSTEM *FS)                                                                   
*                                                                              
* Variables:                                                                   
* *FS -> The structure containing the file system information                                                                         
*                                                                              
* Description:                                                                 
* This function will display the main information from the MBR and the locations                                                                             
* of different data. This is used to verify that everything is correct.                                                                             
*******************************************************************************/
void FAT32_Display_MBR(FILE_SYSTEM *FS)
{
  printf("************ MBR DATA *************\r\n");
  printf("Bytes/Sector: %u \r\n",FS->sector_size);
  printf("Sectors/Cluster: %u \r\n",FS->sectors_cluster);
  printf("Reserved Sectors: %u \r\n",FS->reserved_sectors);
  printf("FAT Copies: %u \r\n",FS->FAT_copies);
  printf("Sectors/FAT: %lu \r\n",FS->sectors_FAT);
  printf("FAT Start Sector: %lu \r\n",FS->start_FAT);
  printf("Root Dir Cluster: %lu \r\n",FS->root_dir_cluster);
  printf("Root Start: %lu \r\n",FS->root_start);
  printf("Total Sectors: %lu \r\n",FS->total_sectors);
  printf("Total Clusters: %lu \r\n",FS->total_clusters);
  printf("Capacity: %lu \r\n",FS->card_capacity);
  printf("Signature: 0x%X \r\n",FS->signature);
  printf("Partition Start: %.4X \r\n",FS->partition_start);
  printf("\r\n\r\n");
}

/*******************************************************************************
* Function: FAT32_File_Properties(SD_FILE *FILE1)                                                                    
*                                                                              
* Variables:                                                                   
* *FILE1 -> Contains the structure which holds key information about the file                                                                          
*                                                                              
* Description:                                                                 
* This function will display key information about the specified file.         
*******************************************************************************/
void FAT32_File_Properties(SD_FILE *FILE1)
{
  printf("************ FILE DATA *************\r\n");
  printf("Filename: %s \r\n",FILE1->name);
  printf("Start Cluster: %lu \r\n",FILE1->start_cluster);
  printf("Start Sector: %lu \r\n",FILE1->start_sector);
  printf("Total Clusters: %lu \r\n",FILE1->total_clusters);
  printf("Total Sectors: %lu \r\n",FILE1->total_sectors);
  printf("File Size: %lu \r\n",FILE1->size);
  printf("\r\n\r\n");
}

/*******************************************************************************
* Function: FAT32_Display_Data(UINT8 *buf, UINT16 len, UINT8 format)                                                                   
*                                                                              
* Variables:                                                                   
* *buf -> The buffer of data that is to be displayed
* len -> The amount of the buffer to ne displayed      
* formart -> Determines what format the data is displayed in (Hex,char,dec,etc)                                                                         
*                                                                              
* Description:                                                                 
* This function will display a specified amount of bytes from a specified buffer                                                                             
* on a terminal program. The data is shown with each row containing 32 bytes
* of data. Each sector of data (512 bytes) is spaced out from one another. This
* is a good function to visually verify data on the SD card. The format parameter
* allows the user to select the format that the data is displayed in. Hex = 0,
* chars = 1 and dec = 2.                                                                          
*******************************************************************************/
void FAT32_Display_Data(UINT8 *buf, UINT16 len, UINT8 format)
{
  UINT16 i;
  
  //Loop until the specified amount of the buffer has been displayed
  for (i = 0;i < len;i++)
  {
	  //Display the data in the format selected
	  switch (format)
	  {		  
		  //Display a byte in character form
		  case 1: printf("%c",buf[i]); break;
		  
		  //Display a byte in decimal form
		  case 2: printf("%u ",buf[i]); break;
		  
		  //Display a byte in hexadecimal form
		  default: printf("%.2X ",buf[i]); break;
		 } 
		  			    
    //Start a new line every 32 bytes
    if (((i+1) % 32) == 0)
      printf("\r\n");
    
    //Start a new block every 512 bytes
    if (((i+1) % 512) == 0)
      printf("\r\n\r\n");
  }    
  
  //Allow spacing for next function
  printf("\r\n\r\n");
}
  
#endif
