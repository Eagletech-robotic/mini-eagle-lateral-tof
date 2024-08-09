#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "motor_control.h"

Motor motorLeft = {
	.output1_Port = MotorL_output1_GPIO_Port, .output1_Pin = MotorL_output1_Pin, .output2_Port = MotorL_output2_GPIO_Port, .output2_Pin = MotorL_output2_Pin, .enable_Port = MotorL_enable_GPIO_Port, .enable_Pin = MotorL_enable_Pin, .timer3_Channel = TIM_CHANNEL_1, .encoderTimer = TIM5};

Motor motorRight = {
	.output1_Port = MotorR_output1_GPIO_Port, .output1_Pin = MotorR_output1_Pin, .output2_Port = MotorR_output2_GPIO_Port, .output2_Pin = MotorR_output2_Pin, .enable_Port = MotorR_enable_GPIO_Port, .enable_Pin = MotorR_enable_Pin, .timer3_Channel = TIM_CHANNEL_3, .encoderTimer = TIM4};

/** Set motor speed */
void setMotor(Motor *motor, int speed)
{
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
	__HAL_TIM_SET_COMPARE(&htim3, motor->timer3_Channel, abs(speed));
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

	// 1. Move forward
	setMotor(&motorLeft, 50);
	setMotor(&motorRight, 50);
	HAL_Delay(2000); // Run for 2 seconds

	// 2. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);
	HAL_Delay(1000); // Pause for 1 second
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 3. Move backward
	setMotor(&motorLeft, -TIM3_PERIOD);
	setMotor(&motorRight, -TIM3_PERIOD);
	HAL_Delay(2000); // Run for 2 seconds

	// 4. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);
	HAL_Delay(1000); // Pause for 1 second
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 5. Turn left (right motor forward, left motor backward)
	setMotor(&motorLeft, -TIM3_PERIOD * 0.5);
	setMotor(&motorRight, TIM3_PERIOD * 0.5);
	HAL_Delay(1500); // Turn for 1.5 seconds

	// 6. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);
	printEncoderDeltas(&leftCNT, &rightCNT);

	// 7. Blink LED to indicate test completion
	for (int i = 0; i < 3; i++)
	{
		HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
		HAL_Delay(200);
	}
}
