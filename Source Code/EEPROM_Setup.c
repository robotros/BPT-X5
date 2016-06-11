/*******************************************************************************
* Title: EEPROM_Setup.c                                                                      
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                          
* Date: March 13, 2015                                                                              
*                                                                              
* Description:                                                                 
* This file contains the functions that are needed to control the 25LCxxx/25AAxxx
* line of EEPROM ICs from Microchip Inc.                                      
*******************************************************************************/

#ifndef EEPROM_SETUP_C
#define EEPROM_SETUP_C

#include "Main_Includes.h"
#include "EEPROM_Setup.h"
#include "SPI_Setup.h"
#include "Delay_Setup.h"

/*******************************************************************************
* Function: EEPROM_Init(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will put the chip into an inactive state and disable HOLD mode.                                                                             
*******************************************************************************/
void EEPROM_Init(void)
{ 
  //Disable the EEPROM
  EE_CS = 1;
  
  //Make sure the EEPROM is not in HOLD mode
  EE_HOLD = 1;
}

/*******************************************************************************
* Function: EEPROM_Command(UINT8 cmd)                                                                    
*                                                                              
* Variables:                                                                   
* cmd ->  The command that is to be sent to the EEPROM                                                                        
*                                                                              
* Description:                                                                 
* This function will send a single 8-bit command to the EEPROM.                                                                             
*******************************************************************************/
void EEPROM_Command(UINT8 cmd)
{ 
  //Enable the EEPROM
  EE_CS = 0;
  
  //Send command
  EE_SPI_Send(cmd);
  
  //Disable the EEPROM
  EE_CS = 1;
}

/*******************************************************************************
* Function: EEPROM_Write(UINT16 addr, UINT8 *buf, UINT16 len)                                                                   
*                                                                              
* Variables:                                                                   
* addr -> The first address in the EEPROM that is to be written
* *buf -> The buffer that contains the data to be written                                                                         
* len ->  The amount of consecutive bytes to be written from the buffer     
*                                                                      
* Description:                                                                 
* This function will write the specified amount of data in a buffer to the
* specified address and any consecutive locations following it. Each EEPROM has
* a page size determined in bytes, where we can only write an X amount of bytes
* before having to restart the write cycle at the next page address. This function
* will automatically handle any page overruns to ensure that all data is written
* properly. If the operation completes successfully, it will return a 0. Otherwise,
* it will return a non-zero error with a specific error code.                                                                             
*******************************************************************************/
UINT8 EEPROM_Write(UINT16 addr, UINT8 *buf, UINT16 len)
{ 
  UINT16 i;
  UINT16 end_addr;
 
  //Calculate the end address
  end_addr = addr + len;
  
  //If the end address is beyond the last physical address of the chip, return an error
  if (end_addr > EEPROM_CAPACITY)
    return EEPROM_CAPACITY_ERROR;
  
  //Prepare the IC to begin writing data into the EEPROM
  EEPROM_Command(EE_WREN);
  
  //Enable the EEPROM
  EE_CS = 0;
  
  //Send the write command along with the first address of data to be written
  EE_SPI_Send(EE_WRITE);
  EE_SPI_Send((UINT16)addr >> 8);
  EE_SPI_Send(addr & 0x00FF);
  
  //Loop until all data has been written
  for (i = addr;i < end_addr;i++)
  {
    //Clock in a byte of data (Keep in mind that the pointer location increments
    //by 1 each time AFTER this function has ran)
    EE_SPI_Send(*buf++);
    
    //Check to see if the next data address is at the end of a page
    if (((i + 1) % EEPROM_PAGE_SIZE) == 0)
    {
      //Check if all of the data has been written before proceeding
      if ((i + 1) == end_addr)
      {
        //Disable the EEPROM
        EE_CS = 1;
        
        //Return Success
        return EEPROM_SUCCESS;
      }  
      
      //Disable the EEPROM
      EE_CS = 1;
      
      //Wait for last write to finish
      while ((EEPROM_Status() & 0x03) != 0);
      
      //In order to write the next page, we need to send the WREN command
      EEPROM_Command(EE_WREN);
      
      //Enable the EEPROM
      EE_CS = 0;
      
      //Send the write command along with the new page address
      EE_SPI_Send(EE_WRITE);
      EE_SPI_Send((i + 1) >> 8);
      EE_SPI_Send((i + 1) & 0x00FF);
    }  
  }  
  
  //Disable the EEPROM
  EE_CS = 1;
  
  //Return Success
  return EEPROM_SUCCESS;
}

/*******************************************************************************
* Function: EEPROM_Read(UINT16 addr, UINT8 *buf, UINT16 len)                                                                   
*                                                                              
* Variables:                                                                   
* addr -> The first address in the EEPROM that is to be read
* *buf -> The buffer that will store the read data                                                                         
* len ->  The amount of consecutive bytes to be read into the buffer     
*                                                                      
* Description:                                                                 
* This function will read the specified amount of bytes starting at the specified
* address. All of the data will be stored in the buffer pointed to by *buf.                                                                          
*******************************************************************************/
void EEPROM_Read(UINT16 addr, UINT8 *buf, UINT16 len)
{  
  UINT16 i;

  //Enable the EEPROM
  EE_CS = 0;
  
  //Send a read command along with the address of the first byte to be read
  EE_SPI_Send(EE_READ);
  EE_SPI_Send(addr >> 8);
  EE_SPI_Send(addr & 0x00FF);
  
  //Keep reading consecutive bytes until the amount specified has been read
  for (i = 0;i < len;i++)
    buf[i] = EE_SPI_Send(0xFF); 
  
  //Return a success
  EE_CS = 1;
}

/*******************************************************************************
* Function: EEPROM_Status(void)                                                                   
*                                                                              
* Variables:                                                                   
* N/A                                                                          
*                                                                              
* Description:                                                                 
* This function will read the status register of the EEPROM. This is crucial to
* determine when a write is in progress, whether the write enable switch has been
* set or whether there is write protection set on nay of the EEPROM banks.                                                                             
*******************************************************************************/
UINT8 EEPROM_Status(void)
{
  UINT8 response;
  
  //Enable the EEPROM
  EE_CS = 0;
  
  //Send a request to read the STATUS register
  EE_SPI_Send(EE_RDSR);
  
  //Retrieve the bits in the STATUS register 
  response = EE_SPI_Send(0x00);
  
  //Disable the EEPROM
  EE_CS = 1;
  
  //Return the STATUS register values
  return response;
} 
  
#endif
