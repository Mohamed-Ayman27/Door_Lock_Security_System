 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER AVR driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "common_macros.h"
#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_SRC, TIMER_PRE_1, TIMER_PRE_8, TIMER_PRE_64, TIMER_PRE_256, TIMER_PRE_1024, EXT_FALL, EXT_RISE
}Timer1_Prescaler;

typedef enum
{
	TIMER_NORMAL,TIMER_CTC
}Timer1_Mode;


typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);


/*
 * Description:
 * Function to disable the Timer1
 */
void Timer1_deInit(void);

/*
 * Description:
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void(*a_ptr)(void));



#endif /* TIMER_H_ */
