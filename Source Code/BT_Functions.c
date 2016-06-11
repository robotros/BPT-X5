/*******************************************************************************
* Title: BT_Functions.c                                                                      
* Version: 1.0                                                                 
* Author: Jeff Nybo                                                            
* Date: March 13, 2015                                                                       
*                                                                              
* Description:                                                                 
* This file contains various routines which control the operation of the UART
* menu and commands. The UART module used in the beer pong table is the HC-06
* bluetooth to UART wireless module. After successfully pairing the module to a
* PC, a user can send commands and receive information from the HC-06 with the                                                                             
* use of a terminal program, such as 'Tera Term'.                                                                             
*******************************************************************************/

#ifndef BT_FUNCTIONS_C
#define BT_FUNCTIONS_C

#include "Main_Includes.h"
#include "BT_Functions.h"
#include "LED_Control.h"
#include "LED_Graphics.h"
#include "Grid_Setup.h"
#include "EEPROM_Setup.h"
#include "Delay_Setup.h"
#include "Interrupts.h"
#include "SD_Setup.h"
#include "FAT32_Setup.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern volatile char uart_str[32];
extern volatile const char UART_CMD[25][32];

extern UINT8 SD_buf[SD_BUF_SIZE];

extern volatile INT16 cal_light[24];
extern volatile UINT16 IR_value[24];

extern volatile UINT32 IR_sensors;

extern volatile RGB COLOR[11];
extern volatile T16_FLAG FLAG1;

extern volatile UINT32 grid_row[12];
extern volatile UINT8 UART_rx_buf[128];

/*******************************************************************************
* Function: void HC06_Init(char name[20], UINT32 baudrate)                                                                 
*                                                                             
* Variables:                                                                   
* name -> The name of the bluetooth module
* baudrate -> The speed of the UART transmission                                                                         
*                                                                             
* Description:                                                                 
* This function is used to describe the operation of the UART command needed to 
* control the RGB pods from a UART connection.                                                                             
*******************************************************************************/
void HC06_Init(char name[20], UINT32 baudrate)
{	
	//Change the HC06 to operate at the specified baudrate. It may already be 
	//configured this way but execute the code below to ensure that it is.
	switch (baudrate)
	{
		case 1200: 		printf("AT+BAUD1"); break;
		case 2400: 		printf("AT+BAUD2"); break;
		case 4800: 		printf("AT+BAUD3"); break;
		case 9600: 		printf("AT+BAUD4"); break;
		case 19200: 	printf("AT+BAUD5"); break;
		case 38400: 	printf("AT+BAUD6"); break;
		case 57600: 	printf("AT+BAUD7"); break;
		case 115200:  printf("AT+BAUD8"); break;
		case 230400:  printf("AT+BAUD9"); break;
		case 460800:  printf("AT+BAUDA"); break;
		case 921600:  printf("AT+BAUDB"); break;
		case 1382400: printf("AT+BAUDC"); break;
		
		//If an incorrect value is specified, revert to a default baudrate of 9600bps
		default:	printf("AT+BAUD4"); break;
	}	 
	
	//Allow a 1s delay so that the HC06 fully accepts this command before we send
	//more commands
	Delay_ms(1000);
	
	//Configure the baudrate for the microcontroller to match the HC06 baudrate
	UART_Set_Baudrate(baudrate);
	Delay_ms(500);
	
	//Send a command to the HC06 to change the name of the bluetooth module
	printf("AT+NAME%s",name);
	Delay_ms(200);
}	

/*******************************************************************************
* Function:                                                                 
*                                                                             
* Variables:                                                                                                                                           
*                                                                             
* Description:           
*                                                                          
*******************************************************************************/  
UINT8 Receive_Buffer_Size(UINT8 *data)
{
	UINT16 CMD = 0;
  
  //Mask off the first 2 bytes which are the COMMAND bytes  //Mask off the first 2 bytes which are the COMMAND bytes
  CMD = COMBINE16(data[0],data[1]);  
  
  //Determine which command is to be carried out
  switch (CMD)
  {
	  case BT_GRID_CONTROL: 			return BT_GRID_CONTROL_RX_BUF; 				break;
	  
	  case BT_TEST_IR_VALUES: 	return BT_TEST_IR_VALUES_RX_BUF; 			break;
	  
	  case BT_ACTIVE: 						return BT_ACTIVE_RX_BUF;  break;
	  case BT_STANDBY: 						return BT_STANDBY_RX_BUF;  break;
	}
	
	return BT_INVALID_COMMAND;
}

/*******************************************************************************
* Function: Check_UART_Command(char str[32])                                                                   
*                                                                             
* Variables:                                                                   
* str[32] -> Should contain the last 32 characters received from the UART module                                                                          
*                                                                             
* Description:                                                                 
* This function will compare the passed in string (str[32]) with a list of commands
* that are defined in Globals.h to see if a specific command has been received from
* the UART module. If a recognized command was received, the function will return a
* (0) and if there is no recognizable command, it will return a non-zero number.                                                                             
*******************************************************************************/  
UINT8 Check_UART_Command(UINT8 *data)
{
	UINT16 CMD = 0;
	int i;
  
	//Reset the scroll finished flag in case it has been set so that we can
	//scroll text if we want too
	SCROLL_FINISHED = 0; 
  
  //Mask off the first 2 bytes which are the COMMAND bytes
  CMD = COMBINE16(data[0],data[1]);  
  
  //Determine which command is to be carried out
  switch (CMD)
  {
	  case BT_GRID_CONTROL: BT_Update_LED_Grid(&data[2]); break;
	  
	  case BT_TEST_IR_VALUES: 	BT_IR_Sensor_Data(); 			break;
	  
	  case BT_ACTIVE: MODE_STANDBY = OFF; break;
	  
	  case BT_STANDBY:  
		    				//Enter standby on next main loop
							  MODE_STANDBY = ON;
							  
							  //Disable all features
		            Disable_All_Features();  break;
	}
}	


/*******************************************************************************
* Function:                                                                 
*                                                                             
* Variables:
* N/A                                                                                                                                           
*                                                                             
* Description:           
*                                                                          
*******************************************************************************/  
void BT_Update_LED_Grid(UINT8 *data)
{
	INT8 i;
	UINT8 p;
	
	for (i = 0;i < 12;i++)
	{
		p = i * 4;
		
		grid_row[i] = COMBINE32(data[p],data[p+1],data[p+2],data[p+3]);
	}
	
	UPDATE_FRAME();	
}		

/*******************************************************************************
* Function:                                                                 
*                                                                             
* Variables:
* N/A                                                                                                                                           
*                                                                             
* Description:           
*                                                                          
*******************************************************************************/  
void BT_IR_Sensor_Data(void)
{
	UINT8 i;
	INT16 diff;
	char state[16];
	
	for (i = 0;i < 20;i++)
	{
		diff = IR_value[i] - cal_light[i];
		
		if (IR_sensors & (1 << i))
			strcpy(state,"Cup Detected");
		else
			strcpy(state,"No Cup Detected");
		
		printf("Pod #%d\r\n",i+1);
		Delay_ms(1);
		
		printf("Light Reading: %d\r\n",IR_value[i]);
		Delay_ms(1);
		
		printf("Offset Amount: %d\r\n",cal_light[i]);
		Delay_ms(1);
		
		printf("Adjusted Level: %d\r\n",diff);
		Delay_ms(1);
		
		printf("Detection State: %s\r\n\r\n",state);
		Delay_ms(1);
	}	
}	

/*******************************************************************************
* Function: Check_UART_Command(char str[32])                                                                   
*                                                                             
* Variables:                                                                   
* str[32] -> Should contain the last 32 characters received from the UART module                                                                          
*                                                                             
* Description:                                                                 
* This function will compare the passed in string (str[32]) with a list of commands
* that are defined in Globals.h to see if a specific command has been received from
* the UART module. If a recognized command was received, the function will return a
* (0) and if there is no recognizable command, it will return a non-zero number.                                                                             
*******************************************************************************//*  
UINT8 Check_UART_Command(char str[32])
{
  INT16 i,j;
  INT16 str_loc = 0xFF;
  UINT8 match;
  char cmd[32];
  UINT32 part = 0;
  UINT16 value = 0;
  
  //Clear UART flag
  UPDATE_UART = 0;
  
  //Automatically convert any lowercase characters to uppercase
  for (i = 0;i < 32;i++)
  	uart_str[i] = toupper(uart_str[i]);
  
  //Check the receive buffer to see if any commands have been sent
  for (i = 30;i >= 0;i--)
  {
	  //If 'B' and 'T' are found in successive order, look for a command
    if (uart_str[i] == 'B' && uart_str[i+1] == 'T')
    {
	    //Save the spot in the string buffer where the command begins
      str_loc = i;
     
      //Pack the characters after the "BT" command into a string 
      for (j = 0;j < (32 - str_loc);j++)
           cmd[j] = uart_str[str_loc+j];
      
      //Exit the loop
      i = -1;
    }  
  } 
  
  //Clear the string buffer so that it can start fresh on next command
  Clear_UART_String();
  
  //No proper command was found in the string, return error
  if (str_loc == 0xFF)
  {
    printf("\r\nUnrecognized Command\r\n\r\n>"); 
    return BT_GENERAL_ERROR;
  }  
  
  //Check for ':' token which is located in a variable command
  i = Find_Char(cmd,':');
  
  //No ':' token was found, proceed to check if a static command was sent
  if (i == 0xFF)
  {
	  //Compare received string with command set
    match = Find_Command(cmd);
    
    //A match was found in the command list, execute the specific static command
    if (UART_Execute_Command(match,part,value) == 0)  
      return BT_SUCCESS;
    
    //The user did not type in a proper command, return error  
    else 
    {
      printf("\r\nUnrecognized Command\r\n\r\n>"); 
      return BT_GENERAL_ERROR;
    }  
  }
  
  //Check for '=' token which is located in a variable command
  j = Find_Char(cmd,'=');
  
  //No '=' token was found which means it is not a variable command, return error
  if (j == 0xFF)
  {
    printf("\r\nUnrecognized Command\r\n\r\n>"); 
    return BT_GENERAL_ERROR;
  }  
  
  //Separate the static command from the command variables. This way we can compare
  //the user input command to the command list in Globals.h properly
  cmd[i] = '\0';
  
  //Convert the first variable in the command to an integer value
  i++;
  part = atol(&cmd[i]);
  
  //Convert the second variable in the command to an integer value
  j++;
  value = atoi(&cmd[j]);
  
  //Check the static command against the command list to see if we have a match for
  //a variable command
  match = Find_Command(cmd);
  
  //A match was found in the command list, execute the specific command
  if (UART_Execute_Command(match,part,value) == 0)  
    return BT_SUCCESS;
    
  //The user did not type in a proper variable command, return error  
  else 
  {
    printf("Unrecognized Command\r\n\r\n>"); 
    return BT_GENERAL_ERROR;
  }  
} 
*/
  
/*******************************************************************************
* Function: Clear_UART_String(void)                                                                   
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function simply clears all 32 elements of the global UART string to '\0'.                                                                             
*******************************************************************************/
void Clear_UART_String(void)
{
  INT8 j;
  
  //Reset all 32 elements to '\0'
  for (j = 0;j < 32;j++)
      uart_str[j] = '\0'; 
} 

/*******************************************************************************
* Function: Debug_Menu(void)                                                                   
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function will simply send the required UART data to print this menu
* in a terminal program.                                                                             
*******************************************************************************/
void Debug_Menu(void)
{ 
	printf("\r\n\r\n"); 
  printf("____________________________________________________________________________________\r\n");
  printf("------------------------------------------------------------------------------------\r\n");
  printf("||                                                                                ||\r\n");
  printf("||                          Interactive LED Beer Pong Table                       ||\r\n");
  printf("||                                   Debugging Menu                               ||\r\n");
  printf("||                                                                                ||\r\n");
  printf("------------------------------------------------------------------------------------\r\n");
  printf("||                                                                                ||\r\n");
  printf("||    CMD ----------------------> Description                                     ||\r\n");
  printf("||    n => Variable #1 (Usually depicts a destination or part #)                  ||\r\n");
  printf("||    x => Variable #2 (Usually used to write/assign a value to a register)       ||\r\n");
  printf("||                                                                                ||\r\n");
  printf("------------------------------------------------------------------------------------\r\n");
  printf("||                                                                                ||\r\n");
  printf("|| BT                  -> Link Test; Will return an Ack signal, verifying link.   ||\r\n");
  printf("|| BT+MENU             -> Show this current menu                                  ||\r\n");
  printf("|| BT+VERSION          -> Will return the current firmware version                ||\r\n");
  printf("|| BT+POD:n=x          -> Activate Pod                                            ||\r\n");
  printf("|| BT+LEDRING:n=x      -> Set LED Ring brightness                                 ||\r\n");
  printf("|| BT+PIXEL:n=x        -> Turn on/off a specified pixel on the LED grid           ||\r\n");
  printf("|| BT+STANDBY          -> Disable all currently running animations                ||\r\n");
  printf("|| BT+ACTIVE           -> Activate all animations and features                    ||\r\n");
  printf("|| BT+HELP             -> A detailed section regarding all commands               ||\r\n");
  printf("|| BT+WRITE+EEPROM:n=x -> Used to write one byte into EEPROM for testing purposes ||\r\n");
  printf("|| BT+READ+EEPROM:n=x  -> Used to verify the data in one reigster of the EEPROM   ||\r\n");
  printf("|| BT+RESET            -> Reset the table; Can be used to upgrade firmware        ||\r\n");
  printf("||                                                                                ||\r\n");
  printf("---------------------------------------------------------------------------------");
}

/*******************************************************************************
* Function: EEPROM_Help_Menu(void)                                                                    
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function is used to describe the operation of the UART command needed to 
* write and read data to and from the 256Kb EEPROM chip.                                                                         
*******************************************************************************/
void EEPROM_Help_Menu(void)
{ 
	printf("\r\n\r\n\r\n"); 
  printf("---------------------------------------------------------------------------------\r\n");
  printf("|                       EEPROM Read/Write Command Help                          |\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("| This command is used to ensure that the PIC is communicating with the EEPROM  |\r\n");
  printf("| properly. To test if we have a solid connection to the EEPROM chip, we can    |\r\n");
  printf("| write one byte to any address (0 - 32767) of the chip. For example, to write  |\r\n");
  printf("| a value of 196 to the address register at 21548 we would write:               |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+WRITE+EEPROM:21548=196                                                     |\r\n");
  printf("|                                                                               |\r\n");
  printf("| To read the value back that was written into the register of 21458, we would  |\r\n");
  printf("| write:                                                                        |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+READ+EEPROM:21548=0                                                        |\r\n");
  printf("|                                                                               |\r\n");
  printf("| The 2nd variable (0) has no effect on the read and can be any value. However, |\r\n");
  printf("| it must still be included in the command or an error will be returned.        |\r\n");
  printf("|                                                                               |\r\n");
  printf("| The best way to test the EEPROM is to choose an address, read that address    |\r\n");
  printf("| to see what default value is in there, then write a new value to that address |\r\n");
  printf("| again and read the value in the address once more, ensuring that it changed   |\r\n");
  printf("| to the value that you just wrote into it.                                     |\r\n");
  printf("---------------------------------------------------------------------------------");
} 
  
/*******************************************************************************
* Function: Find_Char(char str[32], char token)                                                                   
*                                                                             
* Variables:   
* str[32] -> Should contain the parsed command from the UART buffer
* token -> The single char token that is being searched for in 'str[32]'                                                                    
*                                                                             
* Description:                                                                 
* This function will look for a specified token in a string. The token must be
* of type char (1 Byte) and the function will return the location of the character
* in the string if a token is found. Otherwise, it will return a general error.
* If there are more than one tokens in the string, the first tokens location will
* be returned.                                                                             
*******************************************************************************/
UINT8 Find_Char(char str[32], char token)
{
  INT8 i = 0;
  
  //Cycle through all 32 elements of the string
  for (i = 0;i < 32;i++)
  {
	  //If a token is found, returning the token location instantly
    if (str[i] == token)
      return i;
  }
  
  //A token was not found
  return BT_GENERAL_ERROR;    
} 
  
/*******************************************************************************
* Function: Find_Command(char str[32])                                                                   
*                                                                             
* Variables:  
* str[32] -> Should contain the parsed command from the UART buffer                                                                    
*                                                                             
* Description:                                                                 
* This function is called from within Check_UART_Command(str[32]) and will 
* cross-reference the parsed command with the commands stored in 'UART_CMD' which
* is defined in Globals.h. If a match is found, the commands index location in
* the UART_CMD[25][32] array will be returned. Otherwise a general error will be
* returned (0xFF).                                                                             
*******************************************************************************/
UINT8 Find_Command(char str[32])
{
  UINT8 i;
  
  //Check all 25 possible BT commands 
  for (i = 0;i < 25;i++)
  {
	  //If a match has been found, return the commands indexed location in the array
    if (strcmp(str,(const char *)UART_CMD[i]) == 0)
      return i;
  }
  
  //No match found, return an error
  return BT_GENERAL_ERROR;       
}

/*******************************************************************************
* Function: Help_Menu(void)                                                                   
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function will display the following help menu on a terminal program.                                                                             
*******************************************************************************/
void Help_Menu(void)
{ 
	printf("\r\n\r\n\r\n"); 
  printf("_________________________________________________________________________________\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("||                                 Help Menu                                   ||\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("||                                                                             ||\r\n");
  printf("|| BT+POD+HELP      -> Bring up help menu for BT+POD command                   ||\r\n");
  printf("|| BT+LEDRING+HELP  -> Bring up help menu for BT+LEDRING command               ||\r\n");
  printf("|| BT+PIXEL+HELP    -> Bring up help menu for BT+PIXEL command                 ||\r\n");
  printf("|| BT+EEPROM+HELP   -> Bring up the help menu for the EEPROM testing commands  ||\r\n");
  printf("||                                                                             ||\r\n");
  printf("---------------------------------------------------------------------------------");
}

/*******************************************************************************
* Function: LED_Ring_Help_Menu(void)                                                                    
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function is used to describe the operation of the UART command needed to 
* control the LED ring channels (LED1 - LED16 on the PCB) from a UART connection.                                                                             
*******************************************************************************/
void LED_Ring_Help_Menu(void)
{ 
	printf("\r\n\r\n\r\n"); 
  printf("---------------------------------------------------------------------------------\r\n");
  printf("|                         BT+LEDRING:n=x Command Help                           |\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("| This command allows users to change the brightness of the LED rings or any    |\r\n");
  printf("| other devices that are connected to any of the 16 LEDn channels on the PCB.   |\r\n");
  printf("| The 'n' variable designates the LEDn channel and the 'x' variable sets the    |\r\n");
  printf("| brightness level of the channel (0 - 65535).                                  |\r\n");
  printf("|                                                                               |\r\n");
  printf("| To set the channel of LED7 to a PWM value of 26000, we would write:           |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+LEDRING:7=26000                                                            |\r\n");
  printf("|                                                                               |\r\n");
  printf("| To set all LEDn channels to a specific value, enter a value of 999 for 'n'.   |\r\n");
  printf("| To set all channels to a value of 62321 we would write:                       |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+LEDRING:999=62321                                                          |\r\n");
  printf("|                                                                               |\r\n");
  printf("| When you are testing this command out you should disable the main animations  |\r\n");
  printf("| that will be running. Use the command BT+STANDBY to pause operation and allow |\r\n");
  printf("| debugging. BT+ACTIVE will then resume the main animations.                    |\r\n");
  printf("---------------------------------------------------------------------------------");
}

/*******************************************************************************
* Function: Pixel_Help_Menu(void)                                                                    
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function is used to describe the operation of the UART command needed to 
* control pixels on the LED grid from a UART connection.                                                                             
*******************************************************************************/
void Pixel_Help_Menu(void)
{ 
	printf("\r\n\r\n\r\n"); 
  printf("---------------------------------------------------------------------------------\r\n");
  printf("|                          BT+PIXEL:n=x Command Help                            |\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("| This command allows users to enable or disable a specific pixel on the LED    |\r\n");
  printf("| grid. The pixels can be arranged in a grid up to amaximum size of 32x16 LEDs. |\r\n");
  printf("| To calculate the location of a pixel in a grid, use the following equation:   |\r\n");
  printf("|                                                                               |\r\n");
  printf("| n = 1-Dimensional Pixel Location                                              |\r\n");
  printf("|                                                                               |\r\n");
  printf("| n = (y * 32) + x                                                              |\r\n");
  printf("|                                                                               |\r\n");
  printf("| For example, to turn on the pixel at P(6,8) we would write:                   |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+PIXEL:262=1                                                                |\r\n");
  printf("|                                                                               |\r\n");
  printf("| a '1' will turn a pixel on and a '0' will turn a pixel off. To control all    |\r\n");
  printf("| of the LED pixels at a time, enter a value of 999 for 'n'. For example, to    |\r\n");
  printf("| turn all LED pixels off, enter the following:                                 |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+PIXEL:999=0                                                                |\r\n");
  printf("|                                                                               |\r\n");
  printf("| When you are testing this command out you should disable the main animations  |\r\n");
  printf("| that will be running. Use the command BT+STANDBY to pause operation and allow |\r\n");
  printf("| debugging. BT+ACTIVE will then resume the main animations.                    |\r\n");
  printf("---------------------------------------------------------------------------------");
}

/*******************************************************************************
* Function: Pod_Help_Menu(void)                                                                    
*                                                                             
* Variables:                                                                   
* N/A                                                                          
*                                                                             
* Description:                                                                 
* This function is used to describe the operation of the UART command needed to 
* control the RGB pods from a UART connection.                                                                             
*******************************************************************************/
void Pod_Help_Menu(void)
{ 
	printf("\r\n\r\n\r\n"); 
  printf("---------------------------------------------------------------------------------\r\n");
  printf("|                            BT+POD:n=x Command Help                            |\r\n");
  printf("---------------------------------------------------------------------------------\r\n");
  printf("| This command allows the user to change the color of a pod. The pod to be      |\r\n");
  printf("| changed should be one of the 20 pods and denoted by 'n'. The 'x' variable     |\r\n");
  printf("| will set the color of the RGB pod to one of the ten default colors. The color |\r\n");
  printf("| will be represented by a number from 0 - 10. The color representations are as |\r\n");
  printf("| follows:                                                                      |\r\n");
  printf("|                                                                               |\r\n");
  printf("| 0  -> BLACK (OFF)                                                             |\r\n");
  printf("| 1  -> RED                                                                     |\r\n");
  printf("| 2  -> GREEN                                                                   |\r\n");
  printf("| 3  -> BLUE                                                                    |\r\n");
  printf("| 4  -> CYAN                                                                    |\r\n");
  printf("| 5  -> MAGENTA                                                                 |\r\n");
  printf("| 6  -> YELLOW                                                                  |\r\n");
  printf("| 7  -> PINK                                                                    |\r\n");
  printf("| 8  -> ORANGE                                                                  |\r\n");
  printf("| 9  -> VIOLET                                                                  |\r\n");
  printf("| 10 -> WHITE                                                                   |\r\n");
  printf("|                                                                               |\r\n");
  printf("| To set pod #9 to a color of pink, the command would be written as follows:    |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+POD:9=7                                                                    |\r\n");
  printf("|                                                                               |\r\n");
  printf("| To set all pods to a specified color, set the 'n' variable equal to 999. If   |\r\n");
  printf("| we were to set all of the pods to blue, we would write:                       |\r\n");
  printf("|                                                                               |\r\n");
  printf("| BT+POD:999=3                                                                  |\r\n");
  printf("|                                                                               |\r\n");
  printf("| When you are testing this command out you should disable the main animations  |\r\n");
  printf("| that will be running. Use the command BT+STANDBY to pause operation and allow |\r\n");
  printf("| debugging. BT+ACTIVE will then resume the main animations.                    |\r\n");
  printf("---------------------------------------------------------------------------------");
}
  
/*******************************************************************************
* Function: UART_Execute_Command(UINT8 cmd, UINT16 part, UINT16 value)                                                                   
*                                                                             
* Variables:                                                                   
* cmd -> The UART command that is to be executed
* part -> The first parameter, usually defining specific part or address
* value -> The data value that is to be written to the part/address                                                                        
*                                                                             
* Description:                                                                 
* This function is called after a command has been successfully received from 
* the UART. This function calls various commands, but the first parameter will
* usually define the number of a part (such as 7, to define pod #7 with the pod
* command) and the second parameter is the data that is to be written to that
* part. This function is only ever called from within Check_UART_Command(str)
* after a command has been received from the UART.                                                                             
*******************************************************************************/
UINT8 UART_Execute_Command(UINT8 cmd, UINT32 part, UINT16 value)
{
	UINT8 i;
	UINT8 data;

/*	
	//Send a new line command to the terminal
	printf("\r\n");
	
	//Check to see which command has been received
  switch (cmd)
  {
    case BT_ACKNOWLEDGE: 
    				printf("BT Acknowledged\r\n"); break;
    
    case BT_DEBUG_MENU: 
    				Debug_Menu();	break;
    
    case BT_VERSION: 
    				printf("Version "); 
    				printf(BPT_X5_VERSION); 
    				printf("\r\n");	break;
    
    case BT_POD :    
    				//Make sure that the color selection falls between the 11 default colors   
    				if (value > 10)
    				{
	            printf("Color Is Out Of Range (0 - 10) <-> ");
              return BT_GENERAL_ERROR;
            }
            
            //If 'part' equals 999, set all pods to the specified color
            if (part == 999)
            {
            	Set_All_Pods(COLOR[value]);
            	printf("Command Successful\r\n");
            }	
            
            //If 'part' is out of range, return an error
    				else if ((part > 20) || (part < 1))
    				{
	            printf("Part Number Out Of Range (1 - 20) <-> ");
              return BT_GENERAL_ERROR;
            }
            
            //Set the specified pod to the specified color
            else
            {
            	Pod_Set_Color(part,COLOR[value]);
            	printf("Command Successful\r\n");  
            }	break;
    
    case BT_RING:       
            //If 'part' equals 999, set all LED* channels to the specified brightness
            if (part == 999)
            {
            	Set_All_Rings(value,1);
            	printf("Command Successful\r\n");
            }	
            
            //If 'part' is out of range, return an error
    				else if ((part > 16) || (part < 0))
    				{
	            printf("Part Number Out Of Range (1 - 16) <-> ");
              return BT_GENERAL_ERROR;
            }
            
            //Set the specified LEDx channel to the specified brightness
            else
            {
            	Update_Ring(part,value);
            	printf("Command Successful\r\n");  
            } break; 
              
    case BT_PIXEL: 
            //If 'part' equals 999, set all LED* channels to the specified brightness
            if (part == 999)
            {
	            //If 'value' is a 1, turn on all LEDs
            	if (value)
            		Fill_Grid();
            		
	            //If 'value' is a 0, turn off all LEDs
            	else
            		Clear_Grid();
            }	
    				
    				//Set the state of the specified pixel
      			else
    					LED_Pixel(part%32,part/32,value); 
    				
    				//Update the LED grid	 
    				UPDATE_FRAME(); 
    				
            printf("Command Successful\r\n"); break;
    
    case BT_STANDBY:  
    				//Enter standby on next main loop
					  MODE_STANDBY = ON;
					  
					  //Disable all features
            Disable_All_Features();
            printf("Command Successful\r\n"); break;    
    
    case BT_ACTIVE: 
    				//Come out of standby and resume operation
    				MODE_STANDBY = OFF;
            printf("Command Successful\r\n"); break;
    
    case BT_HELP_MENU: 
    				//Show the help menu
    				printf("Command Successful\r\n");
            Help_Menu(); break;
    
    case BT_POD_HELP_MENU:  
    				//Show the RGB pod command help menu
    				printf("Command Successful\r\n");
            Pod_Help_Menu(); break;
    
    case BT_RING_HELP_MENU: 
    				//Show the LED ring command help menu
    				printf("Command Successful\r\n");
            LED_Ring_Help_Menu(); break;
    
    case BT_PIXEL_HELP_MENU: 
    			  //Show the LED grid command help menu
    				printf("Command Successful\r\n");
            Pixel_Help_Menu(); break;
    
    case BT_EEPROM_HELP_MENU: 
    				//Show the EEPROM command help menu
    				printf("Command Successful\r\n");
            EEPROM_Help_Menu(); break;
   
    case BT_EEPROM_WRITE: 
    				//Disable the TMR1 interrupt so that the SPI2 bus does not
    				//get interrupted by an RGB pod update (TLC5955) during an 
    				//EEPROM operation
    				_T1IE = 0;
    				 
    				//Typecast the 'data' parameter into an 8-bit integer. Each 
    				//EEPROM address is only 8-bits in size.
    				data = (UINT8)value;
    				 
    				//Let the user know if the command was successful
    				if (EEPROM_Write(part,&data,1) == 0)
    				   printf("Command Successful\r\n");
    				 
    				//If the command didn't execute properly, inform the user	  
    				else
    				   printf("Error writing EEPROM\r\n");
    				 
    				//Re-enable TMR1 as we are done with the EEPROM and can release 
    		    //the SPI2 bus	so that it can be used for the TLC5955's  
    				_T1IE = 1;  break;
    
    case BT_EEPROM_READ: 
    				//Disable TMR1 so that it won't interrupt the SPI2 bus
    				_T1IE = 0;
    				 
    				//Read and display the specified byte of data from the EEPROM
    				EEPROM_Read(part,&data,1);
    				printf("Data: %d \r\n",data);
    				 	
    				//Re-enable TMR1 so that it can continue updating the RGB pods  
    				_T1IE = 1;  break;
     
    case BT_RESET: 
	    			//Begin a countdown before resetting the PIC
	          printf("Resetting in...\r\n");
	          printf("3\r\n");
	          Delay_ms(1000);
	          
	          printf("2\r\n");
	          Delay_ms(1000);
	          
	          printf("1\r\n");
	          Delay_ms(1000);
	          
	          //We need to do a hard-reset to be able to enter the bootloader
	          //on start-up. I have tried the ASM 'reset' command as well as
	          //jumping to other parts of code, but the PIC can't enter the
	          //bootloader from those software resets. Send an illegal instruction
	          //such as the one below and it will do a hard reset, thus allowing
	          //one to enter the bootloader if they are holding a key on the remote.
	          asm("goto 0x0002"); break;

    case BT_SD_WRITE: 
    				//Temporary disable interrupts so that the SD card operation is not interrupted
    				DISABLE_GLOBAL_INTERRUPTS();
    				    				
            memset(SD_buf,value,4096);
    				 
    				//Let the user know if the command was successful
    				if (SD_Write_Sector(part,SD_buf) == 0)
    				   printf("Command Successful\r\n");
    				 
    				//If the command didn't execute properly, inform the user	  
    				else
    				   printf("Error Writing SD Card\r\n");
    				   
    				//Re-enable interrupts
    				ENABLE_GLOBAL_INTERRUPTS();
    		break;
    
    case BT_SD_READ: 
    				//Temporary disable interrupts so that the SD card operation is not interrupted
    				DISABLE_GLOBAL_INTERRUPTS();
    				 
    				//Let the user know if the command was successful
    				if (SD_Read_Sector(part,SD_buf) == 0)
    				{
    				   printf("Command Successful\r\n");
    				   
    					//Display the data (in hexadecimal) that was located in the sector we just read 
            	FAT32_Display_Data(SD_buf,512,0);
            }    	
    				 
    				//If the command didn't execute properly, inform the user	  
    				else
    				   printf("Error Reading SD Card\r\n");   				
            
    				//Re-enable interrupts
    				ENABLE_GLOBAL_INTERRUPTS();
    				break;
    
    default: 
    				//This shouldn't ever execute but it is here as a fail-safe
 						return BT_GENERAL_ERROR;; break;
  } 
  
  //Start a new line in the terminal
  printf("\r\n>");
  
  //Return successful execution
  return BT_SUCCESS;  
  */ 
}

#endif
