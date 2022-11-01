 /******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dc.h
 *
 * Description: Header file for the DC MOTOR driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/


#ifndef DC_H_
#define DC_H_

#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	STOP,CW,AW
}DcMotor_State;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define Motor_PORT_ID		PORTB_ID
#define Motor_INPUT_PIN1	PIN0_ID
#define Motor_INPUT_PIN2   	PIN1_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state);



#endif /* DC_H_ */
