 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
#define BUZZER_PORT					PORTA_ID
#define BUZZER_PIN					PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Buzzer_init();

void Buzzer_on(void);

void Buzzer_off(void);



#endif /* BUZZER_H_ */
