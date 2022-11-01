/******************************************************************************
 *
 * Application: Human Machine Interface
 *
 * File Name: HIMI.c
 *
 * Description: Source file of the human interface part of the security system
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/


/*******************************************************************************
 *                                  Modules                                    *
 *******************************************************************************/

#include "HIMI.h"



int main(void)
{
	/* Variable to store the pressed key */
	uint8 key_option;

	SREG  |= ( 1 << 7 ); /* Enable Global Interrupts */

	/* Initialize the UART with Configuration */
	UART_ConfigType UART_Config = {EIGHT_BIT, DISABLED, One_Bit,9600};
	UART_init(&UART_Config);

	/* Initialize LCD */
	LCD_init();

	/* Welcome message and Important note */
	LCD_moveCursor(0, 5);
	LCD_displayString("Welcome");
	LCD_moveCursor(1, 0);
	LCD_displayString("'=' to use Enter");

	_delay_ms(WLCOME_WAIT_TIME);
	LCD_clearScreen();
	LCD_moveCursor(0, 0);

	LCD_displayString("Password 5 char");
	_delay_ms(WLCOME_WAIT_TIME);
	LCD_clearScreen();

	/*Set a New Password*/
	HIMI_setNewPass();

	while(1)
	{
		HIMI_mainOptions();

		key_option = KEYPAD_getPressedKey();

		switch (key_option)
		{
		case OPEN_DOOR:
			HIMI_enterPass(); //ask user to input pass

			HIMI_sendCommand(SEND_CHECK_PASS); //See mcu2 to check pass

			HIMI_sendPassword(g_Password);

			HIMI_sendCommand(OPEN_DOOR);

			g_passStatus = HIMI_commandRecieve();

			if(g_passStatus == WRONG_PASS)
			{
				HIMI_wrongPass();
			}
			else if(g_passStatus == OPENING_DOOR)
			{
				HIMI_openDoor();
			}
			break;

		case CHANGE_PASS:
			HIMI_enterPass(); //ask user to input pass

			HIMI_sendCommand(SEND_CHECK_PASS); //See mcu2 to check pass

			HIMI_sendPassword(g_Password);

			HIMI_sendCommand(CHANGE_PASS);

			g_passStatus = HIMI_commandRecieve();

			if(g_passStatus ==  CHANGING_PASSWORD)
			{
				HIMI_setNewPass();
			}
			/* In case the two passwords did not match */
			else if(g_passStatus == WRONG_PASS)
			{
				HIMI_wrongPass();
			}
			break;

		}

	}

}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void HIMI_TimerCallBackProcessing(void)
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
void HIMI_startTimer(void)
{
	/* Setup Timer Configuration */
	Timer1_ConfigType TIMER_Config = { 0 ,7813, TIMER_PRE_1024, TIMER_CTC};

	/* Initialize the Timer */
	Timer1_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer1_setCallBack(HIMI_TimerCallBackProcessing);
}



uint8 HIMI_commandRecieve(void)
{
	while(UART_recieveByte() != READY_SEND);

	UART_sendByte(READY_RECIEVE);

	g_command = UART_recieveByte();

	UART_sendByte(RECIEVE_DONE);

	return g_command;
}

void HIMI_setNewPass()
{
	g_passStatus = PASS_MIS_MATCHED;

	while(g_passStatus == PASS_MIS_MATCHED)
	{
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("**New Password**");
		_delay_ms(WLCOME_WAIT_TIME);

		LCD_clearScreen();
		LCD_displayString("Enter New Pass: ");
		LCD_moveCursor(1, 0);
		HIMI_getPass(g_Password);

		HIMI_sendCommand(READY_SEND_FIRST_PASS);
		HIMI_sendPassword(g_Password);


		LCD_clearScreen();
		LCD_displayString("re-Enter Pass: ");
		LCD_moveCursor(1, 0);
		HIMI_getPass(g_Password);

		HIMI_sendCommand(READY_SEND_SECOND_PASS);
		HIMI_sendPassword(g_Password);

		g_passStatus = HIMI_commandRecieve();

		/* In case the Two Passwords did not match */
		if (g_passStatus == PASS_MIS_MATCHED)
		{
			LCD_clearScreen();
			LCD_displayString("MISMATCHED Pass");
			_delay_ms(WLCOME_WAIT_TIME);
		}
	}
}

void HIMI_sendCommand(uint8 g_command)
{
	UART_sendByte(READY_SEND); //Check mcu2 if ready to receive

	while(UART_recieveByte() != READY_RECIEVE); //exit when Ready to receive

	UART_sendByte(g_command); //Send the command

	while(UART_recieveByte() != RECIEVE_DONE); //polling until all is received
}


void HIMI_sendPassword (uint8 a_Password[])
{
	uint8 i;

	for(i=0 ; i<PASSWORD_LENGTH ; i++)
	{
		UART_sendByte( a_Password [ i ] );
		_delay_ms(SEND_RECEIVE_TIME);
	}

}



void HIMI_getPass(uint8 a_Password[])
{
	LCD_moveCursor(1, 0);

	uint8 counter=0;

	while(counter != PASSWORD_LENGTH)
	{
		g_getPressedKey = KEYPAD_getPressedKey();

		if(g_getPressedKey >=0 && g_getPressedKey <=9)
		{
			a_Password[counter] = g_getPressedKey;
			LCD_displayCharacter('*');
			counter++;
		}
		_delay_ms(KEY_PRESS); //delay for press key
	}

	while ( KEYPAD_getPressedKey() != '=' ); //Don't exit this function until i press '='
}

void HIMI_mainOptions(void)
{
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("+ : Open Door"); /* Display the first option */
	LCD_moveCursor(1,0); /* Move to the next line */
	LCD_displayString("- : Change Pass"); /* Display the second option */
}

void HIMI_enterPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter Password: ");
	HIMI_getPass(g_Password);

}

void HIMI_openDoor(void)
{
	HIMI_startTimer();

	/* OPEN the door for ( 15 sec ) */
	LCD_clearScreen();
	LCD_displayString("Door is Opening");
	while(g_tick < OPEN_ClOSE_DOOR_TIME); /* Count up to 10 */
	g_tick = 0;

	/* HOLD the door for ( 3 sec ) */
	LCD_clearScreen();
	LCD_displayString("DOOR HOLD");
	LCD_displayStringRowColumn(1, 0, "plz ENTER");
	while(g_tick < HOLD_DOOR_TIME); /* Count up to 3 */
	g_tick = 0;

	/* CLOSE the door for ( 10 sec ) */
	LCD_clearScreen();
	LCD_displayString("Door is Closing");
	while(g_tick < OPEN_ClOSE_DOOR_TIME); /* Count up to 10 */
	g_tick = 0;

	Timer1_deInit();
	LCD_clearScreen();

}

void HIMI_wrongPass(void)
{
	g_passwordMistakes++;

	LCD_clearScreen();
	LCD_displayString("* WRONG PASS *");
	_delay_ms(WLCOME_WAIT_TIME);

	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		HIMI_startTimer();

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 3, "ERORR!!!");

		LCD_displayStringRowColumn(1, 0, "WRONG 3 TIMES");

		while(g_tick != ERORR_TIME); //Delay 60 sec

		/* Reset the counters */
		g_passwordMistakes = 0;
		g_tick = 0;

		Timer1_deInit(); /* Stop the timer */
	}

	LCD_clearScreen();
}


