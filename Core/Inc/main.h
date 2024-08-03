/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_nucleo.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

#define TIM3_PERIOD 0xFFFF
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BSP_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define MotorL_encoder1_Pin GPIO_PIN_0
#define MotorL_encoder1_GPIO_Port GPIOA
#define MotorL_encoder2_Pin GPIO_PIN_1
#define MotorL_encoder2_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define GreenLED_Pin GPIO_PIN_5
#define GreenLED_GPIO_Port GPIOA
#define MotorR_output2_Pin GPIO_PIN_5
#define MotorR_output2_GPIO_Port GPIOC
#define MotorR_output1_Pin GPIO_PIN_12
#define MotorR_output1_GPIO_Port GPIOB
#define MotorL_enable_Pin GPIO_PIN_6
#define MotorL_enable_GPIO_Port GPIOC
#define MotorR_enable_Pin GPIO_PIN_8
#define MotorR_enable_GPIO_Port GPIOC
#define MotorL_output2_Pin GPIO_PIN_11
#define MotorL_output2_GPIO_Port GPIOA
#define MotorL_output1_Pin GPIO_PIN_12
#define MotorL_output1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define MotorR_encoder1_Pin GPIO_PIN_6
#define MotorR_encoder1_GPIO_Port GPIOB
#define MotorR_encoder2_Pin GPIO_PIN_7
#define MotorR_encoder2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
