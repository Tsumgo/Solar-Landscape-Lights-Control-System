#ifndef __LED_H
#define __LED_H
#include "stm32f1xx_hal.h"
//#include "tim.h"
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOC
#define LED_A0_Pin GPIO_PIN_10
#define LED_A0_GPIO_Port GPIOC
#define LED_A1_Pin GPIO_PIN_11
#define LED_A1_GPIO_Port GPIOC
#define LED_A2_Pin GPIO_PIN_12
#define LED_A2_GPIO_Port GPIOC
#define LED_DS_Pin GPIO_PIN_3
#define LED_DS_GPIO_Port GPIOB
#define LED_LCLK_Pin GPIO_PIN_4
#define LED_LCLK_GPIO_Port GPIOB
#define LED_SCK_Pin GPIO_PIN_5
#define LED_SCK_GPIO_Port GPIOB


void HAL_Delay_us(uint16_t us);
void LED_Wei(uint8_t num);
void LED_Write_Data(uint8_t duan,uint8_t wei);
void LED_Refresh(void);
#endif



