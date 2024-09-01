#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "motor_control.h"
#include "stm32f401xe.h"

typedef struct
{
	GPIO_TypeDef *output1_Port;
	uint16_t output1_Pin;
	GPIO_TypeDef *output2_Port;
	uint16_t output2_Pin;
	GPIO_TypeDef *enable_Port;
	uint16_t enable_Pin;
	uint32_t timer3_Channel;
	TIM_TypeDef *encoderTimer;
} Motor;

Motor motorLeft = {
	.output1_Port = MotorL_output1_GPIO_Port, .output1_Pin = MotorL_output1_Pin, .output2_Port = MotorL_output2_GPIO_Port, .output2_Pin = MotorL_output2_Pin, .enable_Port = MotorL_enable_GPIO_Port, .enable_Pin = MotorL_enable_Pin, .timer3_Channel = TIM_CHANNEL_1, .encoderTimer = TIM5};

Motor motorRight = {
	.output1_Port = MotorR_output1_GPIO_Port, .output1_Pin = MotorR_output1_Pin, .output2_Port = MotorR_output2_GPIO_Port, .output2_Pin = MotorR_output2_Pin, .enable_Port = MotorR_enable_GPIO_Port, .enable_Pin = MotorR_enable_Pin, .timer3_Channel = TIM_CHANNEL_3, .encoderTimer = TIM4};

/*
 Set motor speed
 @param motorNb: MOTOR_LEFT or MOTOR_RIGHT
 @param speed: a value between -1 (full speed reverse) and 1 (full speed forward)
 */
void setMotor(int motorNb, float speed)
{
	Motor *motor;
	if (motorNb == MOTOR_LEFT)
		motor = &motorLeft;
	else if (motorNb == MOTOR_RIGHT)
		motor = &motorRight;
	else
	{
		printf("Invalid motor number: %d\r\n", motorNb);
		return;
	}

	if (speed > 1 || speed < -1)
	{
		printf("Invalid speed: %f\r\n", speed);
		return;
	}

	if (speed > 0)
	{
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
						  GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
						  GPIO_PIN_RESET);
	}
	else if (speed < 0)
	{
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
						  GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
						  GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
						  GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
						  GPIO_PIN_RESET);
	}

	__HAL_TIM_SET_COMPARE(&htim3, motor->timer3_Channel, abs((int32_t)(speed * TIM3_PERIOD)));
}

void printEncoderDeltas(uint16_t *leftCNT, uint16_t *rightCNT)
{
	int16_t leftDelta = motorLeft.encoderTimer->CNT - *leftCNT;
	int16_t rightDelta = motorRight.encoderTimer->CNT - *rightCNT;
	printf("Left encoder delta: %d, Right encoder delta: %d\r\n", leftDelta, rightDelta);
	*leftCNT = motorLeft.encoderTimer->CNT;
	*rightCNT = motorRight.encoderTimer->CNT;
}

/** Play a simple demo sequence to confirm that motors are working correctly. */
void testMotors()
{
	uint16_t leftCNT = motorLeft.encoderTimer->CNT;
	uint16_t rightCNT = motorRight.encoderTimer->CNT;

	printf("Motor test starting...\r\n");
	HAL_Delay(2000);

	// 1. Move forward
	setMotor(MOTOR_LEFT, 1.0);
	setMotor(MOTOR_RIGHT, 1.0);
	HAL_Delay(1000);

	// 2. Stop
	setMotor(MOTOR_LEFT, 0.0);
	setMotor(MOTOR_RIGHT, 0.0);
	HAL_Delay(1000);
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 3. Move backward
	setMotor(MOTOR_LEFT, -1.0);
	setMotor(MOTOR_RIGHT, -1.0);
	HAL_Delay(1000);

	// 4. Stop
	setMotor(MOTOR_LEFT, 0.0);
	setMotor(MOTOR_RIGHT, 0.0);
	HAL_Delay(1000);
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 5. Turn left (right motor forward, left motor backward)
	setMotor(MOTOR_LEFT, -0.5);
	setMotor(MOTOR_RIGHT, 0.5);
	HAL_Delay(1500);

	// 6. Stop
	setMotor(MOTOR_LEFT, 0);
	setMotor(MOTOR_RIGHT, 0);
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 7. Blink LED to indicate test completion
	for (int i = 0; i < 3; i++)
	{
		HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
		HAL_Delay(200);
	}

	printf("Motor test complete.\r\n");
}
