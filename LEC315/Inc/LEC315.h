/**
  ******************************************************************************
  * @file           : LEC315.h
  * @author			: Mohamed Maher
  * @brief          : Header for LEC315.c file.
  *                   This file contains the common defines of the application.
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




/* Define to prevent recursive inclusion ----*/
#ifndef  __LEC315_H__
#define  __LEC315_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------*/
#include <stdint.h>
#include <math.h>
#include "stm32f1xx_hal.h"


/* Macros-----------------------------------*/

// change these macros if your compass is not in factory defaults
#define DEVICE_ID 	 0x77
#define DEVICE_ADD	 0x00



/* Public type dev struct  -----------------*/

// TypeDef for all read commands sent to compass by the user 
typedef struct 
{
    uint8_t cmd ;                       // Command value for the corsponding datasheet 
    uint8_t respond_data_length ;       // the compass respond data length
    uint8_t respond_flag ;              // the compass respond command value for the sent command by user
}READ_CMD;

// TypeDef for all SET commands sent to compass by the user

typedef struct 
{
    uint8_t set_cmd ;                   // Command value for the corsponding datasheet 
    uint8_t set_data_len;               // the length of set command data     
    uint8_t respond_data_length ;       // the compass respond data length
    uint8_t respond_flag ;              // the compass respond command value for the sent command by user
}SET_CMD;


/* Public typeDesf enum -----------------*/

typedef enum { false, true } bool;


/* Functions -----------------------------*/


/****************************************************
 * get_crc
 * function to calc check sum value for each command frame
 * sent to the compass
 *
 ****************************************************/
uint8_t get_crc( uint8_t * arr, uint8_t lenght);


/****************************************************
 * LCE315_READ
 * main method to send commands to LEC315 Compass
 * and returns the respond data in suitable format
 * Available commmands:
 * 			- READ_PITCH
 * 			- READ_ROLL
 * 			- READ_AZIM
 * 			- READ_MAGNETIC_DEC
 * 			- READ_COMPASS_ADDR
 * 			- SAVING_SETTINGS
 ****************************************************/
float  LCE315_READ(READ_CMD *command );


/****************************************************
 * LCE315_SET
 * main method to send SET commands to LEC315 Compass
 * and returns "True" in success False in "Fail"
 * Available SET Commands:
 * 	>>> For the passed data argument , refer to datasheet
 * 			- SET_CMD SET_BUADRATE
 * 			- SET_COMPASS_ADDR
 * 			- SET_OUTPUT_MODE
 ****************************************************/
bool LEC315_SET (SET_CMD *command, uint8_t  set_data);


/****************************************************
 * respond_format
 * Takes the data respond and translate it to integer
 * using datasheet formating for each command respond
 *
 ****************************************************/
float respond_format(uint8_t * data, READ_CMD *command );


/****************************************************
 * hex2int
 * take a hex string and convert it to a 32bit number
 * (max 8 hex digits)
 ****************************************************/
uint32_t hex2int(char *hex , uint8_t len);

/****************************************************
 * fualt_blink
 * used as error traping handler
 * MCU will go into infinity loop with fast blinking led tell reseting
 ****************************************************/
void fualt_blink (uint32_t Delay);


#endif
