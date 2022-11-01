 /******************************************************************************
 *
 * File Name: HIMI.h
 *
 * Description: Header file for the HIMI
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#ifndef HIMI_H_
#define HIMI_H_

#include <avr/io.h>
#include <util/delay.h>

#include "common_macros.h"
#include "std_types.h"
#include "keypad.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
#define READY_SEND						0xF0
#define READY_RECIEVE					0xF1
#define RECIEVE_DONE					0xF2

#define READY_SEND_FIRST_PASS			0xF3
#define READY_SEND_SECOND_PASS			0xF4

#define SEND_CHECK_PASS					0xF5
#define WRONG_PASS						0xF6
#define OPENING_DOOR					0xF7
#define CHANGING_PASSWORD				0xF8


#define OPEN_DOOR 						'+'
#define CHANGE_PASS 					'-'



#define WLCOME_WAIT_TIME				1500
#define SEND_RECEIVE_TIME				10
#define KEY_PRESS						500
#define ERORR_TIME           			60
#define OPEN_ClOSE_DOOR_TIME			10
#define HOLD_DOOR_TIME					3



#define PASSWORD_LENGTH					5
#define MAX_NUM_OF_MISTAKES     		3
#define PASS_MIS_MATCHED              	0
#define PASS_MATCHED				  	1

#define TWI_ADDRESS    0b0000001
/*******************************************************************************
 *                      Global Variables                                 	   *
 *******************************************************************************/
uint8 g_Password[PASSWORD_LENGTH] = {0};

uint8 g_getPressedKey;

uint8 g_passStatus;

uint8 g_command;

uint8 g_passwordMistakes = 0;

uint8 g_tick = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void HIMI_TimerCallBackProcessing(void);

void HIMI_startTimer(void);

uint8 HIMI_commandRecieve(void);

void HIMI_setNewPass();

void HIMI_sendCommand(uint8 g_command);

void HIMI_sendPassword (uint8 a_Password[]);

void HIMI_getPass(uint8 a_Password[]);

void HIMI_mainOptions(void);

void HIMI_enterPass(void);

void HIMI_wrongPass(void);

void HIMI_openDoor(void);
#endif /* HIMI_H_ */
