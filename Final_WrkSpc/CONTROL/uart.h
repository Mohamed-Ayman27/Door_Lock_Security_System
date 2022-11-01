 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
  FIVE_BIT , SIX_BIT , SEVEN_BIT , EIGHT_BIT , NINE_BIT = 7
} Character_Size ;

typedef enum
{
	DISABLED,EN_EVEN=2,EN_ODD
}Parity_Mode;

typedef enum
{
	ASYNCH,SYNCH
}Op_Mode;

typedef enum
{
	One_Bit,Two_Bit
}Stop_Bit;

typedef struct
{
	Character_Size CharSize;
	Parity_Mode Parity;
	Stop_Bit  Stop;
	uint32 Baud_Rate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */

void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */

void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #





#endif /* UART_H_ */
