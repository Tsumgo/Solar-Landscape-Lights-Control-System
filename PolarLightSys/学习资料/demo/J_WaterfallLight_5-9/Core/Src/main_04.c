/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

V04、数码管显示时间，jinxd，230702
	1、KEY0，控制正计数还是倒计数；
	2、KEY2，控制运行/暂停；

V03、按键控制流水灯，jinxd，230702
	1、KEY0，控制正反向转动；
	2、KEY1，速度递减；KEY_UP，控制速度增加；
	3、KEY3，控制运行/停止

V02、按键扫描，jinxd，230702
	1、采用定时中断的10ms触发扫描
	2、每个按键对应LED灯的翻转

V01、流水灯，jinxd，230702
	1、改用定时中断方式，定时1ms
	2、采用标志用于周期级与基本级之间的参数传递

V00、流水灯，jinxd，230702
	1、实现流水灯
	2、采用延时函数
	4、可改变流水灯的速度


  ******************************************************************************
  */  
  
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

uint8_t KEY_Scan(uint8_t mode);
void	Tone(uint16_t DelayV);
void LED_Write_Data(uint8_t duan,uint8_t wei);
void LED_Refresh(void);



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	uint8_t flag_1=0;			//流水灯定时标志
	uint8_t	flag_KS =0;			//按键扫描标志
	uint8_t	flag_WLRun =1;		//流水灯运行标志
	uint8_t	flag_Forward =0;	//流水灯方向标志
	uint8_t	flag_DSP =0;		//数码管显示处理，每5ms进行
	
	uint16_t CountTimer =0;
	uint16_t CountTimer10ms =0;
	uint8_t	KeyV =0;			//取得键值
	uint16_t Delayms_V =50;
	uint8_t	LedState =0;
	uint8_t	LedTemp =0;

	uint8_t smg_wei=0;//数码管位选
	uint8_t smg_duan=0;//数码管段选
	uint16_t t=0;
	uint8_t	DSPBuff[8] ={1,2,16,4,5,16,7,8};	//数码管显存
	//共阴数字数组
	//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,全灭
	uint8_t smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,
	0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x02,0x00};
	uint8_t	Time10ms=0,TimeS=0,TimeM=0;		//计时器



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  
  HAL_TIM_Base_Start_IT(&htim1);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//---------------------------------------------------------流水灯处理
	if (flag_1)
	{
		flag_1 = 0;
		if (flag_WLRun)
			{
			LedState++;
			LedState &= 0x07;			//限定状态范围
			if (flag_Forward)
				{
				LedTemp =1;
				LedTemp =LedTemp << LedState;	//依据状态确定点灯的位置
				}
			else
				{
				LedTemp =0x80;
				LedTemp =LedTemp >> LedState;
				}
			HAL_GPIO_WritePin(GPIOC,0xff,GPIO_PIN_SET);			//清除灯	
			HAL_GPIO_WritePin(GPIOC,LedTemp,GPIO_PIN_RESET);	//点灯
			}
	}
	  
	//---------------------------------------------------------按键处理
	if (flag_KS)
		{
		KeyV = KEY_Scan(0);				//按键扫描
		 switch (KeyV)
        {
            case KEY0_PRES://KEY0
            Tone(50);
			flag_Forward ++;
			flag_Forward &=0x01;
				break;

            case KEY1_PRES://KEY1
            Tone(50);
			if (Delayms_V < 200)
				{
				Delayms_V +=5;
				}
                break;

            case WKUP_PRES://控制BEEP
            Tone(50);
			if (Delayms_V > 5)
				{
				Delayms_V -=5;
				}
            break;

            case KEY2_PRES://KEY_UP
            Tone(50);
			flag_WLRun ++;
			flag_WLRun &=0x01;
                break;

            default:
				break;

        }
	
		}


	//---------------------------------------------------------显示处理
	if (flag_DSP)
		{
		flag_DSP =0;

		smg_wei++;
		smg_wei &=0x07;
		//if(smg_wei==8) smg_wei=0;

		smg_duan =DSPBuff[smg_wei];
		LED_Write_Data(smg_num[smg_duan],smg_wei);//数码管显示
		LED_Refresh();//刷新显示

		if (smg_wei ==7)
			{
			DSPBuff[0] =TimeM /10;
			DSPBuff[1] =TimeM %10;
			
			DSPBuff[3] =TimeS /10;
			DSPBuff[4] =TimeS %10;

			DSPBuff[6] =Time10ms /10;
			DSPBuff[7] =Time10ms %10;
		}
	}
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  
  
  
  
  
  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin
                          |LED4_Pin|LED5_Pin|LED6_Pin|LED7_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_A0_Pin|LED_A1_Pin|LED_A2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_DS_Pin|LED_LCLK_Pin|LED_SCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BELL_GPIO_Port, BELL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED0_Pin LED1_Pin LED2_Pin LED3_Pin
                           LED4_Pin LED5_Pin LED6_Pin LED7_Pin
                           LED_A0_Pin LED_A1_Pin LED_A2_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin
                          |LED4_Pin|LED5_Pin|LED6_Pin|LED7_Pin
                          |LED_A0_Pin|LED_A1_Pin|LED_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : WK_UP_Pin */
  GPIO_InitStruct.Pin = WK_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(WK_UP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY0_Pin KEY1_Pin */
  GPIO_InitStruct.Pin = KEY0_Pin|KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY2_Pin */
  GPIO_InitStruct.Pin = KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_DS_Pin LED_LCLK_Pin LED_SCK_Pin BELL_Pin */
  GPIO_InitStruct.Pin = LED_DS_Pin|LED_LCLK_Pin|LED_SCK_Pin|BELL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


//-----------------------------------------------------------单音信号
void	Tone(uint16_t DelayV)
{
	HAL_GPIO_WritePin(BELL_GPIO_Port,BELL_Pin,GPIO_PIN_RESET);	
	HAL_Delay(DelayV);
	HAL_GPIO_WritePin(BELL_GPIO_Port,BELL_Pin,GPIO_PIN_SET);
}


//-----------------------------------------------------------TIM1中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance ==TIM1)
	{
//	  flag_1 = 1;
	CountTimer ++;
	CountTimer10ms ++;
	
	  if(CountTimer > Delayms_V)	//"Delayms_V",置流水灯刷新标志
	  {
	  	flag_1 = 1;
		CountTimer = 0;  
	  }

	  if (CountTimer10ms >9)		//10ms，置按键扫描标志
		  {
		  CountTimer10ms =0;
		  flag_KS =1;
			
			if(flag_Forward) 
				{if(flag_WLRun) Time10ms --;
				if (Time10ms ==50)
					{
					DSPBuff[2] =17;		//显示“   ”
					DSPBuff[5] =17;
					}

				if (Time10ms ==255)
					{
					Time10ms =99;

					DSPBuff[2] =16;		//显示“-”
					DSPBuff[5] =16;

					TimeS --;			//秒表计时
					
					if (TimeS ==255)
						{
						TimeS =59;
						TimeM --;			//秒表计时
						if (TimeM ==255)
							{
							TimeM =59;
							}
						}
					}
				}			//秒表计时
			else 
				{if(flag_WLRun) Time10ms ++;
				if (Time10ms ==50)
					{
					DSPBuff[2] =17; 	//显示“   ”
					DSPBuff[5] =17;
					}
				
				if (Time10ms >99)
					{
					Time10ms =0;
				
					DSPBuff[2] =16; 	//显示“-”
					DSPBuff[5] =16;
				
					TimeS ++;			//秒表计时
					
					if (TimeS >59)
						{
						TimeS =0;
				
						TimeM ++;			//秒表计时
						if (TimeM >59)
							{
							TimeM =0;
							}
						}

					}
				}
	  }

	if (CountTimer10ms & 0x01)	//2ms，置数码显示处理标志
		{
		flag_DSP =1;
		}

	}

	
}	


//-----------------------------------------------------------按键处理函数
//返回按键值
//mode：0,不支持连续按;1,支持连续按;
//返回值：//0，没有任何按键按下//KEY0_PRES，KEY0按下//KEY1_PRES，KEY1按下//KEY2_PRES，KEY2按下//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	
	if (key_up)
	{					//有按键按下？
		if(KEY0 ==0)
		{
			key_up=0;
			return KEY0_PRES;
		}
		else if(KEY1 ==0)
		{
			key_up=0;
			return KEY1_PRES;
		}
		else if(KEY2 ==0)
		{
			key_up=0;
			return KEY2_PRES;
		}
		else if(WK_UP ==1)
		{
			key_up=0;
			return WKUP_PRES;
		}
	}
	else if(KEY0==1 && KEY1==1 && KEY2==1 && WK_UP==0) key_up=1;	//检查是否所有按键弹起？
		return 0;

}

//-----------------------------------------------------------数码管显示处理函数
void HAL_Delay_us(uint16_t us)
{
	uint16_t	Temp;

	if (us--)
		{
			for (Temp = 0; Temp < 72; ++Temp)
			{
			}
		}
}

//74HC138 驱动
//数码管位选
//num:要显示的数码管编号 0-7(共 8 个数码管)
void LED_Wei(uint8_t num)
{
 	HAL_GPIO_WritePin(LED_A0_GPIO_Port,LED_A0_Pin,(GPIO_PinState)(num&0x01));
	HAL_GPIO_WritePin(LED_A1_GPIO_Port,LED_A1_Pin,(GPIO_PinState)((num&0x02)>>1));
	HAL_GPIO_WritePin(LED_A2_GPIO_Port,LED_A2_Pin,(GPIO_PinState)((num&0x04)>>2));
}


//74HC595 驱动
//数码管显示
//duan:显示的段码
//wei:要显示的数码管编号 0-7(共 8 个数码管)
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
 LED_Wei(wei);//后选中位
}


//74HC595 驱动
//数码管刷新显示
void LED_Refresh(void)
{
	HAL_GPIO_WritePin(LED_LCLK_GPIO_Port,LED_LCLK_Pin,GPIO_PIN_SET);
	HAL_Delay_us(5);
	HAL_GPIO_WritePin(LED_LCLK_GPIO_Port,LED_LCLK_Pin,GPIO_PIN_RESET);
}








/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
