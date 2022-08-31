
/**
  ******************************************************************************
  * @file           : LEC315.c
  * @author			: Mohamed Maher
  * @brief          : Driver for BWsensing LEC315 3D Compass
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 www.infinitytech.ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the MIT LICENSE

  *
  ******************************************************************************
  */

/* Includes ---------------------------------*/


#include <stdint.h>
#include <stdlib.h>

#include "LEC315.h"
#include "main.h"

UART_HandleTypeDef huart3;

/* LEC315 COMPASS COMMANDS DELCERATIONS -------*/

// Reading Commands" --  = {cmd_value, respond_data_length, respond_flag }
READ_CMD READ_PITCH          = {0x01, 3, 0x81};
READ_CMD READ_ROLL           = {0x02, 3, 0x82};
READ_CMD READ_AZIM           = {0x03, 3, 0x83};
//READ_CMD READ_ALL_ANGLES     = {0x04, 9, 0x84};
READ_CMD READ_MAGNETIC_DEC   = {0x07, 2, 0x87};
READ_CMD READ_COMPASS_ADDR   = {0x1F, 1, 0x1F};
READ_CMD SAVING_SETTINGS     = {0x0A, 1, 0x8A};

// SET Commands" --  --------------------------*/
//SET COMMANDS --- SIZE = 6:7 Bytes
SET_CMD SET_BUADRATE        = {0x0B, 1, 0X0B, 1};
//SET_CMD SET_MAGNETIC_DEC    = {0x06, 2, 0X86, 1};
SET_CMD SET_COMPASS_ADDR    = {0x0F, 1, 0X8F, 1};  // REFER TO DATA SHEET FOR MORE DETAILS 
SET_CMD SET_OUTPUT_MODE     = {0x0C, 1, 0X8C, 1};   


/****************************************************
 * get_crc
 * function to calc crc value for each command frame
 * sent to the compass
 *
 ****************************************************/
uint8_t get_crc( uint8_t * arr, uint8_t lenght)
{
    uint8_t sum =0;
    for (uint8_t i=0; i<lenght-1; i++)
        sum += arr[i] ;
    return sum;
}

/****************************************************
 * LCE315_READ
 * main method to send commands to LEC315 Compass
 * and returns the respond data in suitable format
 *
 ****************************************************/
float LCE315_READ(READ_CMD *command )
{
    // cmd_frame ={id, length, address, cmd, crc}
    uint8_t frame_len = 0x05;
    uint8_t cmd_frame [5] = {DEVICE_ID, (frame_len-1), DEVICE_ADD, command->cmd, get_crc(cmd_frame, frame_len) };
    uint8_t current_respond_flag ;

    /*  Different commands gets variable length respond data
     *  So we have to dynamic allocate a data buffer according to the sent command
     */
    uint8_t *data_buff = calloc(command->respond_data_length, sizeof(uint8_t));
    if(data_buff == NULL){
    	 /* USER Error handler here  */
    	fualt_blink (100);
    }

    // UART send command frame to the compass
    if(HAL_UART_Transmit(&huart3, cmd_frame, frame_len, 500 ) > 0){
    	 /* USER Error handler here  */
    	fualt_blink (200);

    }


	// keep in receiving loop till you get the respond flag and read the data
	while (1)
	{
		if(HAL_UART_Receive(&huart3, current_respond_flag, 1, 500) > 0){
			/* USER Error handler here  */
			fualt_blink (200);
		}


		if( current_respond_flag == command->respond_flag)
		{

			if(HAL_UART_Receive(&huart3, data_buff, command->respond_data_length, 1000) > 0)
			{
				/* USER Error handler here  */
				fualt_blink (200);
			}

			return respond_format(data_buff, command->cmd );
			break ;
		}

	}

    free(data_buff);
    return 0 ;
}


/****************************************************
 * LCE315_SET
 * main method to send SET commands to LEC315 Compass
 * and returns "True" in success False in "Fail"
 *
 ****************************************************/
bool LEC315_SET (SET_CMD *command, uint8_t set_data)
{
	// constructing command frame according to diff SET commands
	uint8_t frame_len = 0x06;
	uint8_t respond_data ;
	uint8_t current_respond_flag ;
	uint8_t set_frame ={DEVICE_ID, (frame_len-1), DEVICE_ADD, command->set_cmd ,set_data, get_crc(set_frame, frame_len) };

	// Sending SET frame to the compass
	// UART send command frame to the compass
	    if(HAL_UART_Transmit(&huart3, set_frame, frame_len, 500 ) > 0){
	    	 /* USER Error handler here  */
	    	fualt_blink (200);

	    }

		// keep in receiving loop till you get the respond flag and read the data
		while (1)
		{
			if(HAL_UART_Receive(&huart3, current_respond_flag, 1, 500) > 0){
				/* USER Error handler here  */
				fualt_blink (200);
			}
			// check if respond frame flag is correct
			if( current_respond_flag == command->respond_flag)
			{
				if(HAL_UART_Receive(&huart3, respond_data, command->respond_data_length, 1000) > 0)
				{
					/* USER Error handler here  */
					fualt_blink (200);
				}
				if(respond_data ==0x00 )
					return true ;
				else
					return false;
			}
		}

}




/****************************************************
 * respond_format
 * Takes the data respond and translate it to integer
 * using datasheet formating for each command respond
 *
 ****************************************************/

float respond_format(uint8_t * data, READ_CMD *command ){

	signed int respond = 0;

	if(command->respond_data_length == 2)
	{
		// respond data format is SXXX -- for details check datasheet
		char intString[4] ;
		sprintf(intString,"%X%X", data[0],data[1]);
		// checking that data sign S = 1: negative else positive
		if (intString[0] > 0)
			return hex2int(intString,4)*(-1);
		else
			return hex2int(intString,4);
	}

	if(command->respond_data_length == 3)
	{
		// respond data format is SXXX.YY -- for details check datasheet
		float result = 0.0 ;
		char intString [4];
		char floatString [2] ;
		//calculating SXXX part
		sprintf(intString,"%X%X", data[0],data[1]);
		result = hex2int(intString,4);

		//calculating .YY part
		sprintf(floatString,"%X", data[2]);
		result = result + (hex2int(floatString,2)/100);

		// checking that data sign S = 1: negative else positive
		if (intString[0] > 0)
			return result*(-1);
		else
			return hex2int(intString,4);
	}

}


/****************************************************
 * hex2int
 * take a hex string and convert it to a 32bit number
 * (max 8 hex digits)
 ****************************************************/

uint32_t hex2int(char *hex , uint8_t len) {
    uint32_t val = 0;

    for (uint8_t i =1 ; i < len ; i++){

        uint8_t byte = hex[i];
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }

    return val;
}

/****************************************************
 * fualt_blink
 * used as error traping handler
 * MCU will go into infinity loop with fast blinking led tell reseting
 ****************************************************/
void fualt_blink (uint32_t Delay){
	while(1)
	{
		HAL_GPIO_TogglePin(fualt_GPIO_Port,fualt_Pin);
		HAL_Delay(Delay);
	}
}
