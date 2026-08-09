/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../App/Include/status.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN1_Pin GPIO_PIN_4
#define BTN1_GPIO_Port GPIOE
#define BTN2_Pin GPIO_PIN_5
#define BTN2_GPIO_Port GPIOE
#define BTN3_Pin GPIO_PIN_6
#define BTN3_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define FAN_Pin GPIO_PIN_0
#define FAN_GPIO_Port GPIOA
#define FAN_A_Pin GPIO_PIN_1
#define FAN_A_GPIO_Port GPIOA
#define FAN_B_Pin GPIO_PIN_2
#define FAN_B_GPIO_Port GPIOA
#define NTC_Pin GPIO_PIN_4
#define NTC_GPIO_Port GPIOA
#define RGB_RED_Pin GPIO_PIN_11
#define RGB_RED_GPIO_Port GPIOE
#define RGB_GREEN_Pin GPIO_PIN_13
#define RGB_GREEN_GPIO_Port GPIOE
#define RGB_BLUE_Pin GPIO_PIN_14
#define RGB_BLUE_GPIO_Port GPIOE
#define LCD_RW_Pin GPIO_PIN_10
#define LCD_RW_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_11
#define LCD_RS_GPIO_Port GPIOD
#define LCD_D7_Pin GPIO_PIN_12
#define LCD_D7_GPIO_Port GPIOD
#define LCD_D6_Pin GPIO_PIN_13
#define LCD_D6_GPIO_Port GPIOD
#define LCD_D5_Pin GPIO_PIN_14
#define LCD_D5_GPIO_Port GPIOD
#define LCD_D4_Pin GPIO_PIN_15
#define LCD_D4_GPIO_Port GPIOD
#define LCD_EN_Pin GPIO_PIN_7
#define LCD_EN_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
