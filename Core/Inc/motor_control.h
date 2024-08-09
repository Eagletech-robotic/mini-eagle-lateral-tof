#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

typedef struct {
	GPIO_TypeDef *output1_Port;
	uint16_t output1_Pin;
	GPIO_TypeDef *output2_Port;
	uint16_t output2_Pin;
	GPIO_TypeDef *enable_Port;
	uint16_t enable_Pin;
	uint32_t timer3_Channel;
	TIM_TypeDef *encoderTimer;
} Motor;

extern Motor motorLeft;
extern Motor motorRight;

void setMotor(Motor *motor, int speed);
void testMotors(void);

#endif /* INC_MOTOR_CONTROL_H_ */
