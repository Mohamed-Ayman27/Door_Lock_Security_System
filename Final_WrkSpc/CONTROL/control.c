/******************************************************************************
 *
 * CONTROL ECU
 *
 * File Name: control.c
 *
 * Description: Source file for the CONTROL ECU
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#include "control.h"

/*******************************************************************************
 *                      Global Variables                                 	   *
 *******************************************************************************/
uint8 g_passStatus = PASS_MIS_MATCHED;

uint8 g_storedPassword [PASSWORD_LENGTH];

uint8 g_recievedPassword [PASSWORD_LENGTH];

uint8 g_confirmPassword [PASSWORD_LENGTH];

uint8 g_command;

uint8 g_tick = 0;

uint8  g_passwordMistakes = 0;

/*******************************************************************************
 *                      		CODE                                 	   	   *
 *******************************************************************************/


int main(void)
{

	uint8 key_option;

	SREG  |= ( 1 << 7 ); /* Enable Global Interrupts */

	TWI_ConfigType TWI_Config = {TWI_ADDRESS , TWI_PRE_1 , FastMode_400Kb};
	TWI_init(&TWI_Config);

	/* Initialize the UART with Configuration */
	UART_ConfigType UART_Config = {EIGHT_BIT, DISABLED, One_Bit,9600};
	UART_init(&UART_Config);

	DcMotor_Init();

	Buzzer_init();


	CONTROL_setNewPass();
	while(1)
	{
		while( CONTROL_commandRecieve() != SEND_CHECK_PASS);

		CONTROL_recievePass(g_recievedPassword);

		key_option = CONTROL_commandRecieve();

		switch(key_option)
		{
		case OPEN_DOOR:
			/* Read Password from the EEPROM */
			CONTROL_readPassword(g_storedPassword);
			/* Compare it with the inputed user */
			g_passStatus = CONTROL_comparePasswords(g_recievedPassword, g_storedPassword);

			/* In case the two passwords matches */
			if(g_passStatus == PASS_MATCHED)
			{
				/* Send Opening Door command to HMI MCU */
				CONTROL_sendCommand(OPENING_DOOR);
				/* Start Opening Door sequence */
				CONTROL_openDoor();
			}
			/* In case the two passwords did not match */
			else if(g_passStatus == PASS_MIS_MATCHED)
			{
				/* Send Wrong Password command to HMI MCU */
				CONTROL_sendCommand(WRONG_PASS);
				/* Start Wrong Password sequence */
				CONTROL_wrongPass();
			}
			break;
		case CHANGE_PASS:
			/* Read Password from the EEPROM */
			CONTROL_readPassword(g_storedPassword);
			/* Compare it with the inputed user */
			g_passStatus = CONTROL_comparePasswords(g_recievedPassword, g_storedPassword);

			/* In case the two passwords matches */
			if(g_passStatus == PASS_MATCHED)
			{
				CONTROL_sendCommand(CHANGING_PASSWORD);

				CONTROL_setNewPass();
			}
			/* In case the two passwords did not match */
			else if(g_passStatus == PASS_MIS_MATCHED)
			{
				/* Send Wrong Password command to HMI MCU */
				CONTROL_sendCommand(WRONG_PASS);
				/* Start Wrong Password sequence */
				CONTROL_wrongPass();
			}

			break;
		}



	}

}

void CONTROL_TimerCallBackProcessing(void)
{
	g_tick++; /* Increment the counter */
}



/*
 * Description :
 * Produce a delay timer using Polling Technique
 * F_CPU = 8MHz
 * PreScaler = 1024
 * CTC ode
 * Compare Value= 7812.5
 */

void CONTROL_startTimer(void)
{
	/* Setup Timer Configuration */
	Timer1_ConfigType TIMER_Config = { 0 ,7813, TIMER_PRE_1024, TIMER_CTC};

	/* Initialize the Timer */
	Timer1_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer1_setCallBack(CONTROL_TimerCallBackProcessing);
}



void CONTROL_sendCommand(uint8 g_command)
{
	UART_sendByte(READY_SEND); //Check mcu2 if ready to receive

	while(UART_recieveByte() != READY_RECIEVE); //exit when Ready to receive

	UART_sendByte(g_command); //Send the command

	while(UART_recieveByte() != RECIEVE_DONE); //polling until all is received
}


uint8 CONTROL_commandRecieve(void)
{
	while(UART_recieveByte() != READY_SEND);

	UART_sendByte(READY_RECIEVE);

	g_command = UART_recieveByte();

	UART_sendByte(RECIEVE_DONE);

	return g_command;
}

void CONTROL_recievePass(uint8 a_Password[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		a_Password[counter] = UART_recieveByte();
		_delay_ms(SEND_RECEIVE_TIME);
	}

}

void CONTROL_setNewPass()
{
	g_passStatus = PASS_MIS_MATCHED;

	while(g_passStatus == PASS_MIS_MATCHED)
	{
		while(CONTROL_commandRecieve() != READY_SEND_FIRST_PASS);

		CONTROL_recievePass(g_recievedPassword);

		while(CONTROL_commandRecieve() != READY_SEND_SECOND_PASS);

		CONTROL_recievePass(g_confirmPassword);

		g_passStatus = CONTROL_comparePasswords(g_recievedPassword, g_confirmPassword);

		if( g_passStatus == PASS_MIS_MATCHED)
		{
			CONTROL_sendCommand(PASS_MIS_MATCHED);
		}

		else if( g_passStatus == PASS_MATCHED)
		{
			CONTROL_savePassword(g_recievedPassword);

			CONTROL_sendCommand(PASS_MATCHED);
		}
	}
}

uint8 CONTROL_comparePasswords(uint8 a_password1[], uint8 a_password2[])
{
	uint8 counter;

	for(counter=0 ; counter < PASSWORD_LENGTH ; counter++)
	{
		if( a_password1 [ counter ] != a_password2 [ counter ])
		{
			return PASS_MIS_MATCHED;
		}
	}
	return PASS_MATCHED;
}

void CONTROL_savePassword(uint8 a_password[])
{
	uint8 counter;

	for(counter = 0 ; counter <PASSWORD_LENGTH ; counter++)
	{
		EEPROM_writeByte( ( 0x0311+counter ), a_password[counter]);

		_delay_ms( STORE_TIME );
	}

}

void CONTROL_readPassword(uint8 a_storedPassword[])
{
	uint8 counter;

	for(counter = 0 ; counter <PASSWORD_LENGTH ; counter++)
	{
		EEPROM_readByte( ( 0x0311+counter ), &a_storedPassword[counter]);

		_delay_ms( STORE_TIME );
	}
}

void CONTROL_wrongPass(void)
{

	g_passwordMistakes++;

	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		CONTROL_startTimer();
		Buzzer_on();

		while(g_tick != ERORR_TIME);

		g_tick=0;
		g_passwordMistakes=0;

		Timer1_deInit();
	}
	Buzzer_off();


}

void CONTROL_openDoor(void)
{
	CONTROL_startTimer();

	DcMotor_Rotate(CW);
	while(g_tick != OPEN_ClOSE_DOOR_TIME);
	g_tick=0;


	DcMotor_Rotate(STOP);
	while(g_tick != HOLD_DOOR_TIME);
	g_tick=0;


	DcMotor_Rotate(AW);
	while(g_tick != OPEN_ClOSE_DOOR_TIME);
	g_tick=0;


	DcMotor_Rotate(STOP);

	Timer1_deInit();
}


