#include "LED.h"

void HAL_Delay_us(uint16_t us)
{
	uint16_t	Temp;

	if (us--)
		{
			for (Temp = 0; Temp < 72; ++Temp)
			{
			}
		}

	/*
uint16_t differ =0xffff-us-5;
	HAL_TIM_Base_Start(&htim2);
  __HAL_TIM_SetCounter(&htim2,differ);
  while(differ<0xffff-5)
{
	differ=__HAL_TIM_GetCounter(&htim2);
}
	HAL_TIM_Base_Stop(&htim2);
*/
}

//74HC138 驱动
//数码管位�?
//num:要显示的数码管编�?0-7(�?8 个数码管)
void LED_Wei(uint8_t num)
{
 	HAL_GPIO_WritePin(LED_A0_GPIO_Port,LED_A0_Pin,(GPIO_PinState)(num&0x01));
	HAL_GPIO_WritePin(LED_A1_GPIO_Port,LED_A1_Pin,(GPIO_PinState)((num&0x02)>>1));
	HAL_GPIO_WritePin(LED_A2_GPIO_Port,LED_A2_Pin,(GPIO_PinState)((num&0x04)>>2));
}
//74HC595 驱动
//数码管显�?
//duan:显示的段�?
//wei:要显示的数码管编�?0-7(�?8 个数码管)
void LED_Write_Data(uint8_t duan,uint8_t wei)
{
uint8_t i;
for( i=0;i<8;i++)//先送段
{
  HAL_GPIO_WritePin(LED_DS_GPIO_Port,LED_DS_Pin,(GPIO_PinState)((duan>>i)&0x01));
  HAL_GPIO_WritePin(LED_SCK_GPIO_Port,LED_SCK_Pin,GPIO_PIN_RESET);
  HAL_Delay_us(5);
  HAL_GPIO_WritePin(LED_SCK_GPIO_Port,LED_SCK_Pin,GPIO_PIN_SET);
}
 LED_Wei(wei);//后选中�?
}
//74HC595 驱动
//数码管刷新显�?
void LED_Refresh(void)
{
  HAL_GPIO_WritePin(LED_LCLK_GPIO_Port,LED_LCLK_Pin,GPIO_PIN_SET);
  HAL_Delay_us(5);
	HAL_GPIO_WritePin(LED_LCLK_GPIO_Port,LED_LCLK_Pin,GPIO_PIN_RESET);

}
