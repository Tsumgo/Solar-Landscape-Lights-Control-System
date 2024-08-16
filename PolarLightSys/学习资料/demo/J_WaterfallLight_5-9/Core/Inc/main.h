/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOC
#define WK_UP_Pin GPIO_PIN_0
#define WK_UP_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOC
#define LED5_Pin GPIO_PIN_5
#define LED5_GPIO_Port GPIOC
#define LED6_Pin GPIO_PIN_6
#define LED6_GPIO_Port GPIOC
#define LED7_Pin GPIO_PIN_7
#define LED7_GPIO_Port GPIOC
#define KEY0_Pin GPIO_PIN_8
#define KEY0_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_9
#define KEY1_GPIO_Port GPIOC
#define LED_A0_Pin GPIO_PIN_10
#define LED_A0_GPIO_Port GPIOC
#define LED_A1_Pin GPIO_PIN_11
#define LED_A1_GPIO_Port GPIOC
#define LED_A2_Pin GPIO_PIN_12
#define LED_A2_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOD
#define LED_DS_Pin GPIO_PIN_3
#define LED_DS_GPIO_Port GPIOB
#define LED_LCLK_Pin GPIO_PIN_4
#define LED_LCLK_GPIO_Port GPIOB
#define LED_SCK_Pin GPIO_PIN_5
#define LED_SCK_GPIO_Port GPIOB
#define BELL_Pin GPIO_PIN_8
#define BELL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define KEY0        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)  //KEY0按键PC8
#define KEY1        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)  //KEY1按键PC9
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)  //KEY2按键PD2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define KEY0_PRES	1	//KEY0  
#define KEY1_PRES	2	//KEY1 
#define KEY2_PRES	3	//KEY2
#define WKUP_PRES	4	//WK_UP 
//uint8_t KEY_Scan(uint8_t mode);



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
