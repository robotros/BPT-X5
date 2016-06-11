/*******************************************************************************
* Title: SD_Setup.c                                                           
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                             
* Date: March 13, 2015                                                                          
*                                                                              
* Description:                                                                 
* This file contains the functions that are used to initialize and control the 
* operation of an SD/SD Card.                                                 
*******************************************************************************/  
    
#ifndef SD_SETUP_C
#define SD_SETUP_C

#include "Main_Includes.h"
#include "SPI_Setup.h"
#include "SD_Setup.h"

extern UINT8 SD_buf[SD_BUF_SIZE];
        
/*******************************************************************************
* Function: SD_Command(UINT8 *cmd, UINT8 len)                                                                   
*                                                                              
* Variables:                                                                    
* *cmd -> Points to an array consisting of a command or data                                                                             
* len -> The 8-bit array length                                                                             
*                                                                              
* Description:                                                                 
* This function is used to send commands and data to the SD card. The function                                                                             
* will return the response from the SD card that was received while the last byte
* of *data was sent.                                                                             
*******************************************************************************/
UINT8 SD_Command(UINT8 *data, UINT16 len)
{
  UINT16 i;
  UINT8 response;
  
  //Send commands/data to the SD card  
  for (i = 0;i < len;i++)
   response = SPI_SD_SEND(*data++); 
  
  //Return the last response from the SD card
  return response; 
}    
        
/*******************************************************************************
* Function: SD_Match(UINT8 check)                                                                   
*                                                                              
* Variables:                                                                   
* check -> The value that is to be checked against the SD cards response                                                                          
*                                                                              
* Description:                                                                 
* This function will continuously clock dummy data into the SD card while                                                                              
* monitoring the response from the card. If the response that we are checking for
* is received, a 0 will be returned. If the loop times out, a 1 will be returned.                                                                             
*******************************************************************************/
UINT8 SD_Match(UINT8 check)
{
  //This determines the timeout period of the loop
  UINT16 cnt = 20000;
  
  //Continuously clock in data to the SD card and check its response
  while (SPI_SD_SEND(0xFF) != check)
  {
    //If operation has timed out, return an error
    if (cnt-- == 0x00) 
      return NO_MATCH;
  }  
  
  //A matching response was received from the SD card, return success  
  return SUCCESSFUL_MATCH;  
} 
        
/*******************************************************************************
* Function: SD_Busy(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function is used to delay any further instructions to the SD card until
* it completes its current operation. If the SD card takes too long to complete                                                                             
* an operation (i.e it has malfunctioned), the function will return a time out
* error (1). If the SD card finishes its operation before the time out counter,
* an SD card idle definition will be returned (0).                                                                             
*******************************************************************************/
UINT8 SD_Busy(void)
{
  //This determines the timeout period of the loop
  UINT16 cnt = 0xFFFF;
  
  //Continuously clock in data to the SD card and check its response
  while ((SPI_SD_SEND(0xFF) & 0x01) == 0)
  {
    //If operation has timed out, return error
    if (--cnt == 0)
      return SD_BUSY_TIMEOUT;
  }  
  
  //Card is idle and can accept commands/data, return success   
  return SD_CARD_IDLE;   
} 
           
/*******************************************************************************
* Function:                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
*
*                                                               
*******************************************************************************/
UINT8 SD_Read_CSD(UINT8 *buf)
{
  static UINT8 CMD9[SD_CMD_LEN] = {SD_CMD9,0x00,0x00,0x00,0x00,0xFF};
  INT16 i;
  
  //Enable the SD card
  SD_ENABLE();
  
  //Wait for SD card to complete all operations before proceeding with read 
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  }  
  
  //Alert card to send back CSD values to host   
  SD_Command(CMD9,SD_CMD_LEN);
  
  //Ensure that a read token is received from SD card before filling buffer
  if (SD_Match(0xFE)) 
  {
    //No read token was received, return error
    SD_DISABLE();
    return SD_READ_ERROR;
  }
 
  for (i = 0;i < SD_CSD_SIZE;i++)
    buf[i] = SPI_SD_SEND(0xFF);
  
  //Send two dummy CRC bytes
  SPI_SD_SEND(0xFF);
  SPI_SD_SEND(0xFF);
  
  //Disable the SD card
  SD_DISABLE();
  
  //Return read successful
  return SD_SUCCESS;
}  

           
/*******************************************************************************
* Function: SD_Capacity(void)                                                  
*                                                                              
* Variables:                                                                   
* N/A                                                                    
*                                                                              
* Description:                                                                 
* This function will read the CSD register of the SD card and then parse the
* received data to determine the capacity of the SD card in bytes. The function
* will return the card capcity if successful, but return a SD_CSD_ERROR (0x01) 
* if the CSD registers of the SD card cannot be read. The needed information is 
* located in the SD specification sheet or in short form below.
*
* Note:
* The data from the CSD is received MSB first! i.e. Byte 0 of the buffer contains
* bits <127:120> and byte 15 contains <7:0>.
*
* Variable          Bitfeld
* C_SIZE ---------- <73:62>
* C_SIZE_MULT ----- <49:47>
* READ_BL_LEN ----- <83:80>
*
* Calculations
* MULT = 2^(C_SIZE_MULT+2)
* BLOCKNR = (C_SIZE + 1) * MULT
* BLOCK_LEN = 2^(READ_BL_LEN)
*
* CAPACITY = BLOCKNR * BLOCK_LEN
*                                                           
*******************************************************************************/
UINT32 SD_Capacity(void)
{
  UINT8 csd[16];
  UINT16 c_size_mult;
  UINT16 read_bl_len;
  UINT32 c_size;
  UINT32 capacity;
  
  //Read the CSD registers, if they cannot be read, return an error  
  if (SD_Read_CSD(csd))
    return SD_CSD_ERROR;
  
  //Parse the 16 bytes of data to find the values that we need for each of
  //the following variables. The bitfield and its corresponding variables
  //are all listed in the SD specification v4.10 on page 115.
  c_size = ((csd[6] & 0x03) << 10) | (csd[7] << 2) | ((csd[8] & 0xC0) >> 6);
  c_size_mult = ((csd[9] & 0x03) << 1) | ((csd[10] & 0x80) >> 7);
  read_bl_len = csd[5] & 0x0F;
  
  //Re-use the initial varaibles with the new calculations for MULT and BLOCK_LEN
  c_size_mult = 1 << (c_size_mult + 2);
  read_bl_len = 1 << read_bl_len;
  
  //Calculate the capacity of the card
  capacity = (c_size + 1) * c_size_mult * read_bl_len;
  
  //Return the card capacity
  return capacity; 
}  
           
/*******************************************************************************
* Function: SD_Init(void)                                                     
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will initialize the SD/SD Card so it will be prepared to      
* Read/Write/Erase data. The SD Card is set up with a sector size of 512
* bytes by default.                                                                   
*******************************************************************************/
UINT8 SD_Init(void)
{
  UINT16 i;
  UINT8 CMD0[SD_CMD_LEN] = {SD_CMD0,0x00,0x00,0x00,0x00,0x95};
  UINT8 CMD1[SD_CMD_LEN] = {SD_CMD1,0x00,0x00,0x00,0x00,0xFF};
  
  //Disable the SD card
  SD_DISABLE();
  
  //Send at least 74 clock cycles to start the SD card
  for (i = 0;i < 10;i++)
    SPI_SD_SEND(0xFF);  

  //Enable the SD card
  SD_ENABLE();
  
  //Put the SD card into a RESET state
  SD_Command(CMD0,SD_CMD_LEN);
  
  //Wait for a response of 0x01
  if (SD_Match(0x01)) 
  {
    //Proper response was never received, disable SD card and return error
    SD_CS = 1; 
    return SD_RESET_ERROR;
  }
  
  //Initalize timeout counter
  i = SD_CMD_REPEAT;
  
  //Keep sending initialization command until SD card accepts it or times out
  do
    SD_Command(CMD1,SD_CMD_LEN);
  
  while (SD_Match(0x00) && (i-- > 0));  
  
  //Check if operation timed out
  if (i == 0)
  {
    //Disable SD card and return error
    SD_DISABLE(); 
    return SD_INIT_ERROR;
  }

  //Disable SD card and send extra clock cycles to the SD card
  SD_DISABLE();
  SPI_SD_SEND(0xFF);
  
  //Return a successful initialization
  return SD_SUCCESS;
}           
        
/*******************************************************************************
* Function: SD_Read_Sector(UINT32 sector, UINT8 *buf)                                                                   
*                                                                              
* Variables:                                                                   
* sector -> The sector address that is to be read
* *buf -> The buffer where the data will be stored (Always 512 bytes)                                                                          
*                                                                              
* Description:                                                                 
* This function will read one sector of data from the SD card. The sector that                                                                             
* is read is determined by the address of the variable 'sector'. There is
* always 512 bytes of data read and stored in the buffer, regardless of
* whether all 512 bytes in the sector are used or not.                                                                             
*******************************************************************************/
UINT8 SD_Read_Sector(UINT32 sector, UINT8 *buf)
{ 
  static UINT8 CMD17[SD_CMD_LEN] = {SD_CMD17,0x00,0x00,0x00,0x00,0xFF};
  UINT16 i;
  
  //Calculate the address by multiplying it by 512 (addr = sector * 512 bytes)
  sector <<= 9;
  
  //Ready the address to be sent in the command
  CMD17[1] = (UINT8)(sector >> 24);
  CMD17[2] = (UINT8)(sector >> 16);
  CMD17[3] = (UINT8)(sector >> 8);
  CMD17[4] = (UINT8)(sector);   
  
  //Enable the SD card
  SD_ENABLE();
  
  //Wait for SD card to complete all operations before proceeding with read 
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  }  
  
  //Prepare to read a single block of data  
  SD_Command(CMD17,SD_CMD_LEN);
  
  //Ensure that a read token is received from SD card before filling buffer
  if (SD_Match(0xFE)) 
  {
    //No read token was received, return error
    SD_DISABLE();
    return SD_READ_ERROR;
  }
 
  //Store sector data in 512 8-bit locations of specified buffer
  for (i = 0;i < SD_SECTOR_SIZE;i++)
    buf[i] = SPI_SD_SEND(0xFF);
  
  //Send two dummy CRC bytes
  SPI_SD_SEND(0xFF);
  SPI_SD_SEND(0xFF);
  
  //Disable the SD card
  SD_DISABLE();
  
  //Return read successful
  return SD_SUCCESS;
} 
        
/*******************************************************************************
* Function: SD_Read_Mul_Sectors(UINT32 start, UINT32 amount)                                                                   
*                                                                              
* Variables:                                                                   
* start -> Address of first sector to be read
* amount -> The amount of consecutive sectors to be read                                                                         
*                                                                              
* Description:                                                                 
* This function will read the specified amount of sectors starting from the                                                                             
* address of the 'start' variable. If this function is to be used with a small
* buffer, the user will have to add code to parse the received data before 
* overwriting it with new data from the next sector(s).                                                                              
*******************************************************************************/
UINT8 SD_Read_Mul_Sectors(UINT32 start, UINT32 amount)
{
  static UINT8 CMD12[SD_CMD_LEN] = {SD_CMD12,0x00,0x00,0x00,0x00,0xFF};
  static UINT8 CMD18[SD_CMD_LEN] = {SD_CMD18,0x00,0x00,0x00,0x00,0xFF};
  UINT32 j;
  UINT16 i;
  
  //Calculate start address
  start <<= 9;
  
  //Inject address into command array
  CMD18[1] = (UINT8)(start >> 24);
  CMD18[2] = (UINT8)(start >> 16);
  CMD18[3] = (UINT8)(start >> 8);
  CMD18[4] = (UINT8)(start);
  
  //Enable the SD card
  SD_ENABLE();
  
  //Wait for SD card to complete all operations before proceeding with read 
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  }  
  
  //Send command to read multiple sectors consecutively
  SD_Command(CMD18,SD_CMD_LEN);
  
  //Loop until all sectors have been read
  for (j = 0;j < amount;j++)
  { 
    //Ensure that a read token is received from SD card before filling buffer
    if (SD_Match(0xFE)) 
    {
      //No read token was received, return error
      return SD_READ_ERROR;
    }
   
    //Store sector data in first 512 bytes of SD_buf[SD_BUF_SIZE]
    for (i = 0;i < SD_SECTOR_SIZE;i++)
      SD_buf[i+(j*SD_SECTOR_SIZE)] = SPI_SD_SEND(0xFF);
    
    //Send two dummy CRC bytes
    SPI_SD_SEND(0xFF);
    SPI_SD_SEND(0xFF);         
    
    /***************************************************
    * Add code to parse sector data here or modify code to fill
    * up a large enough buffer (if you have enough RAM)
    ****************************************************/
  }
  
  //Send stop token to cease read operation
  SD_Command(CMD12,SD_CMD_LEN);
  
  //Send one more dummy byte to allow card to finish operations
  SPI_SD_SEND(0xFF);
  
  //Disable SD card
  SD_DISABLE();
  
  //Send one more dummy byte to allow card to finish operations
  SPI_SD_SEND(0xFF);
  
  //Return successful read
  return SD_SUCCESS;
}  
        
/*******************************************************************************
* Function: SD_Write_Sector(UINT32 sector, UINT8 *buf)                                                                   
*                                                                              
* Variables:            
* sector -> The sector address that is to be written
* *buf -> The buffer where the data will be stored (Always 512 bytes)                                                                                                        
*                                                                              
* Description:                                                                 
* This function will write 512 bytes worth of data (stored in *buf) to the address                                                                            
* that is specified by the 'sector' variable.                                                                             
*******************************************************************************/
UINT8 SD_Write_Sector(UINT32 sector, UINT8 *buf)
{   
  static UINT8 CMD24[SD_CMD_LEN] = {SD_CMD24,0x00,0x00,0x00,0x00,0xFF};
  UINT8 response;
    
  //Enable SD Card  
  SD_ENABLE();
  
  //Wait for SD card to complete all operations before proceeding 
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  }  
  
  //Calculate sector address
  sector <<= 9;
  
  //Inject sector address into command data
  CMD24[1] = sector >> 24;
  CMD24[2] = sector >> 16;
  CMD24[3] = sector >> 8;
  CMD24[4] = sector & 0x000000FF;
  
  //Ready the SD card to accept data
  SD_Command(CMD24,SD_CMD_LEN);
  
  //Wait for a response of 0x00
  if (SD_Match(0x00)) 
  {
    //If operation timed out, return error
    SD_CS = 1; 
    return SD_CMD24_ERROR;
  }
  
  //Send token to begin writing
  SPI_SD_SEND(0xFE);
  
  //Write 512 bytes of data
  response = SD_Command(buf,SD_SECTOR_SIZE);
  
  //Send two dummy CRC bytes 
  SPI_SD_SEND(0xFF);
  SPI_SD_SEND(0xFF);
  
  //Get response from SD card to see if data was accepted
  response = SPI_SD_SEND(0xFF); 
  if ((response & 0x0F) != 0x05) 
  {
    //Data wasn't accepted, disable card and return write error
    SD_DISABLE(); 
    return SD_WRITE_ERROR;
  }   
  
  //Disable SD card
  SD_DISABLE();
  
  //Send one more dummy byte to allow card to finish operations
  SPI_SD_SEND(0xFF);
  
  //Return successful write
  return SD_SUCCESS;
}   
        
/*******************************************************************************
* Function: SD_Write_Mul_Sectors(UINT32 start, UINT32 amount)                                                                   
*                                                                              
* Variables:             
* start -> Address of first sector to be written
* amount -> The amount of consecutive sectors to be written                                                                               
*                                                                              
* Description:                                      
* This function will write the specified amount of sectors starting from the                                                                             
* address of the 'start' variable. If this function is to be used with a small
* buffer, the user will have to add code to update the buffer data in between
* each sector write.                                                           
*******************************************************************************/
UINT8 SD_Write_Mul_Sectors(UINT32 start, UINT32 amount)
{
  static UINT8 CMD25[SD_CMD_LEN] = {SD_CMD25,0x00,0x00,0x00,0x00,0x95};
  UINT8 response;
  UINT32 j;
  
  //Enable SD card
  SD_ENABLE();
  
  //Wait for SD card to complete all operations before proceeding
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  }  
  
  //Calculate sector address
  start <<= 9;
  
  //Inject sector address into command data
  CMD25[1] = start >> 24;
  CMD25[2] = start >> 16;
  CMD25[3] = start >> 8;
  CMD25[4] = start & 0x000000FF;
      
  //Ready the SD card to accept data
  SD_Command(CMD25,SD_CMD_LEN);
  
  //Wait for a response of 0x00
  if (SD_Match(0x00)) 
  {
    //If operation timed out, return error
    SD_CS = 1; 
    return SD_CMD25_ERROR;
  }
  
  //Loop until all sectors have been written
  for (j = 0;j < amount;j++)
  {  
    //Wait for SD card to complete all operations before proceeding
    if (SD_Busy())
    {
      //Disable card and return error on time out
      SD_DISABLE();
      return SD_BUSY_TIMEOUT;
    }  
    
    //Send a start transmission token
    SPI_SD_SEND(0xFC);
  
    //Write 512 bytes of data
    SD_Command(SD_buf,SD_SECTOR_SIZE);
   
    //Send two dummy CRC bytes
    SPI_SD_SEND(0xFF);
    SPI_SD_SEND(0xFF);
    
    //Get response from card
    response = SPI_SD_SEND(0xFF);
    
    //Check if card accepted data 
    if ((response & 0x1F) != 0x05) 
    {
      //Card didn't accept data, return error
      SD_DISABLE(); 
      return SD_WRITE_ERROR;
    } 
    
    /***************************************************
    * Add code to update buffer data here for each write
    ****************************************************/ 
  }
  
  //Wait for SD card to complete all operations before proceeding
  if (SD_Busy())
  {
    //Disable card and return error on time out
    SD_DISABLE();
    return SD_BUSY_TIMEOUT;
  } 
  
  //Send stop token    
  SPI_SD_SEND(0xFD);
  
  //Disable SD card  
  SD_DISABLE();
  
  //Multiple write was successful
  return SD_SUCCESS;
}  
      
/*******************************************************************************
* Function: SD_Erase_Sectors(UINT32 start, UINT32 amount)                                                                   
*                                                                              
* Variables:  
* start -> Address of first sector to be erased
* amount -> The amount of consecutive sectors to be erased                                                                   
*                                                                              
* Description:                                                                 
* This function will erase the specified consecutive sectors on the SD card, replacing                                                                             
* the sector with either (0x00) or (0xFF), depending on the manufacturer.                                                                             
*******************************************************************************/
UINT8 SD_Erase_Sectors(UINT32 start, UINT32 amount)
{
  static UINT8 CMD32[SD_CMD_LEN] = {SD_CMD32,0x00,0x00,0x00,0x00,0x95};
  static UINT8 CMD33[SD_CMD_LEN] = {SD_CMD33,0x00,0x00,0x00,0x00,0x95};
  static UINT8 CMD38[SD_CMD_LEN] = {SD_CMD38,0x00,0x00,0x00,0x00,0x95};

  //Calcaulate the last sector address to be erased
  amount = (start + amount) - 1;
  
  //Multiply each value by 512 to find the offset location (512 bytes per sector)
  start <<= 9;
  amount <<= 9;
  
  //Inject the start sector address into the command
  CMD32[1] = start >> 24;
  CMD32[2] = start >> 16;
  CMD32[3] = start >> 8;
  CMD32[4] = start & 0x000000FF;
  
  //Inject the end sector address into the command
  CMD33[1] = amount >> 24;
  CMD33[2] = amount >> 16;
  CMD33[3] = amount >> 8;
  CMD33[4] = amount & 0x000000FF;
  
  //Enable the SD card
  SD_ENABLE(); 
  
  //Send the address of the first sector to be erased
  SD_Command(CMD32,SD_CMD_LEN);
  
  //Check if the card accepted the command
  if (SD_Match(0x00)) 
  {
    //The command was not accepted, disable the SD card
    SD_CS = 1; 
    
    //Return an error
    return SD_START_ERASE_ERROR;
  }
  
  //Disable SD card
  SD_DISABLE();  
  
  //Re-enable SD card for next command
  SD_ENABLE(); 
  
  //Send the address of the last sector to be erased
  SD_Command(CMD33,SD_CMD_LEN);
  
  //Check if the card accepted the command
  if (SD_Match(0x00)) 
  {
    //The command was not accepted, disable the SD card
    SD_CS = 1; 
    
    //Return an error
    return SD_END_ERASE_ERROR;
  }
  
  //Disable the SD card
  SD_DISABLE();  
  
  //Re-enable the SD card for the next command
  SD_ENABLE(); 
  
  //Send start erase command
  SD_Command(CMD38,SD_CMD_LEN);
  
  //Check if the card accepted the command
  if (SD_Match(0x00)) 
  {
    //The command was not accepted, disable the SD card
    SD_CS = 1; 
    
    //Return an error
    return SD_ERASE_ERROR;
  }
  
  //Disable the SD card
  SD_DISABLE();
    
  //Allow a few more clock cycles for the SD card to finish operations
  SPI_SD_SEND(0xFF);
  
  //Return successful erase
  return SD_SUCCESS;
}  

#endif
