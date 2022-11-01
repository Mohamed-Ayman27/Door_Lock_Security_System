 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer AVR driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/

#include <avr/interrupt.h> /* For Timer ISR */
#include <avr/io.h>	/* For Register names */
#include "timer.h"

/*******************************************************************************
 *                      Global Variables                                 	   *
 *******************************************************************************/

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                      		ISR                                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	/* Call the Call Back function in the application after the edge is detected */
	if( g_callBackPtr != NULL_PTR)
	{
		( *g_callBackPtr )(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}

}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description:
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	/*	TCCR1A:
	 * 		1. COM1A1:0 = 0 0 Normal operation OC1A Disconnected
	 * 		2. COM1B1:0 = 0 0 Normal operation OC1B Disconnected
	 * 		3. FOC1A, B =  1  It's a non-PWM mode
	 * 		4. WGM 11:0 = 0 0 They are set to zero in CTC and normal
	 */
	TCCR1A |= (1<< FOC1A) | (1<<FOC1B);

	/*	TCCR1B:
	 * 		1. ICNC1    = 0
	 * 		2. ICES1    = 0
	 * 		4. WGM12    = Configurable between CTC and normal
	 * 		5. CS12:0   = Clock Configuration
	 */

	TCCR1B = Config_Ptr->prescaler; // Sets the Configuration prescaler
	TCCR1B = ( TCCR1B & (0XF7) ) | ( ( (Config_Ptr)->mode ) << WGM12 ); // Mode Select

	TCNT1  = Config_Ptr->initial_value;

	if( ( (Config_Ptr)->mode ) == TIMER_CTC)
	{
		/*
		 *	1. Configure to the set compare value
		 *	2. Output Compare A match interrupt is enabled
		 */
		OCR1A = Config_Ptr->compare_value;
		TIMSK = (1<<OCIE1A);
	}
	else if( ( (Config_Ptr)->mode ) == TIMER_NORMAL)
	{
		/* Enable overflow flag */
		TIMSK = (1<<TOIE1);
	}
}

/*
 * Description:
 * Function to disable the Timer1
 */
void Timer1_deInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A  = 0;
	TIMSK  = 0;
}

/*
 * Description:
 * Function to set the Call Back function address
 */

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
