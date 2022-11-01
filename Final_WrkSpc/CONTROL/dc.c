/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dc.c
 *
 * Description: Source file for the DC MOTOR driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/

#include "dc.h"

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(Motor_PORT_ID, Motor_INPUT_PIN1, PIN_OUTPUT);
	GPIO_setupPinDirection(Motor_PORT_ID, Motor_INPUT_PIN2, PIN_OUTPUT);

	GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN1,LOGIC_LOW);
	GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN2,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state)
{
	if(state == AW)
	{
		GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN1,LOGIC_HIGH);
		GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN2,LOGIC_LOW);
	}
	else
	{


		GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN1,(state & (0x02) ) );
		GPIO_writePin(Motor_PORT_ID,Motor_INPUT_PIN2,(state & (0x01) ) );
	}


}


