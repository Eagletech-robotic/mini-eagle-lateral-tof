#include <stdlib.h>
#include "main.h"
#include "motor_control.h"

Motor motorLeft = { .output1_Port = MotorL_output1_GPIO_Port, .output1_Pin =
MotorL_output1_Pin, .output2_Port = MotorL_output2_GPIO_Port, .output2_Pin =
MotorL_output2_Pin, .enable_Port = MotorL_enable_GPIO_Port, .enable_Pin =
MotorL_enable_Pin, .timer3_Channel = TIM_CHANNEL_1 };

Motor motorRight = { .output1_Port = MotorR_output1_GPIO_Port, .output1_Pin =
MotorR_output1_Pin, .output2_Port = MotorR_output2_GPIO_Port, .output2_Pin =
MotorR_output2_Pin, .enable_Port = MotorR_enable_GPIO_Port, .enable_Pin =
MotorR_enable_Pin, .timer3_Channel = TIM_CHANNEL_3 };

/** Set motor speed */
void setMotor(Motor *motor, int speed) {
	if (speed > 0) {
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
				GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
				GPIO_PIN_RESET);
	} else if (speed < 0) {
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
				GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(motor->output1_Port, motor->output1_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->output2_Port, motor->output2_Pin,
				GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim3, motor->timer3_Channel, abs(speed));
}

/** Play a simple demo sequence to confirm that motors are working correctly. */
void testMotors() {
	// 1. Move forward
	setMotor(&motorLeft, 100);
	setMotor(&motorRight, 100);
	HAL_Delay(2000);  // Run for 2 seconds

	// 2. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);
	HAL_Delay(1000);  // Pause for 1 second

	// 3. Move backward
	setMotor(&motorLeft, -100);
	setMotor(&motorRight, -100);
	HAL_Delay(2000);  // Run for 2 seconds

	// 4. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);
	HAL_Delay(1000);  // Pause for 1 second

	// 5. Turn left (right motor forward, left motor backward)
	setMotor(&motorLeft, -75);
	setMotor(&motorRight, 75);
	HAL_Delay(1500);  // Turn for 1.5 seconds

	// 6. Stop
	setMotor(&motorLeft, 0);
	setMotor(&motorRight, 0);

	// 7. Blink LED to indicate test completion
	for (int i = 0; i < 3; i++) {
		HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
		HAL_Delay(200);
	}
}
