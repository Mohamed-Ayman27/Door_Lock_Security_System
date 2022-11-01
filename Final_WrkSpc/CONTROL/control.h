 /******************************************************************************
 *
 * CONTROL ECU
 *
 * File Name: control.h
 *
 * Description: Header file for the CONTROL ECU
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#ifndef CONTROL_H_
#define CONTROL_H_

#include <avr/io.h>
#include <util/delay.h>

#include "common_macros.h"
#include "std_types.h"
#include "timer.h"
#include "uart.h"
#include "buzzer.h"
#include "twi.h"
#include "dc.h"
#include "external_eeprom.h"

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
#define STORE_TIME           			80




#define PASSWORD_LENGTH					5
#define MAX_NUM_OF_MISTAKES     		3
#define PASS_MIS_MATCHED              	0
#define PASS_MATCHED				  	1

#define TWI_ADDRESS    0b0000001
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void CONTROL_TimerCallBackProcessing(void);//

void CONTROL_startTimer(void);//

void CONTROL_setNewPass();//

void CONTROL_recievePass(uint8 a_Password[]);//

uint8 CONTROL_comparePasswords(uint8 a_password1[], uint8 a_password2[]);//

void CONTROL_savePassword(uint8 a_receivedPassword[]);//

void CONTROL_readPassword(uint8 a_storedPassword[]);//

void CONTROL_wrongPass(void);//

void CONTROL_openDoor(void);//

uint8 CONTROL_commandRecieve(void);//

void CONTROL_sendCommand(uint8 g_command);//


#endif /* CONTROL_H_ */
