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

V04���������ʾʱ�䣬jinxd��230702
	1��KEY0���������������ǵ�������
	2��KEY2����������/��ͣ��

V03������������ˮ�ƣ�jinxd��230702
	1��KEY0������������ת����
	2��KEY1���ٶȵݼ���KEY_UP�������ٶ����ӣ�
	3��KEY3����������/ֹͣ

V02������ɨ�裬jinxd��230702
	1�����ö�ʱ�жϵ�10ms����ɨ��
	2��ÿ��������ӦLED�Ƶķ�ת

V01����ˮ�ƣ�jinxd��230702
	1�����ö�ʱ�жϷ�ʽ����ʱ1ms
	2�����ñ�־�������ڼ��������֮��Ĳ�������

V00����ˮ�ƣ�jinxd��230702
	1��ʵ����ˮ��
	2��������ʱ����
	4���ɸı���ˮ�Ƶ��ٶ�


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

	uint8_t flag_1=0;			//��ˮ�ƶ�ʱ��־
	uint8_t	flag_KS =0;			//����ɨ���־
	uint8_t	flag_WLRun =1;		//��ˮ�����б�־
	uint8_t	flag_Forward =0;	//��ˮ�Ʒ����־
	uint8_t	flag_DSP =0;		//�������ʾ����ÿ5ms����
	
	uint16_t CountTimer =0;
	uint16_t CountTimer10ms =0;
	uint8_t	KeyV =0;			//ȡ�ü�ֵ
	uint16_t Delayms_V =50;
	uint8_t	LedState =0;
	uint8_t	LedTemp =0;

	uint8_t smg_wei=0;//�����λѡ
	uint8_t smg_duan=0;//����ܶ�ѡ
	uint16_t t=0;
	uint8_t	DSPBuff[8] ={1,2,16,4,5,16,7,8};	//������Դ�
	//������������
	//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,ȫ��
	uint8_t smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,
	0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x02,0x00};
	uint8_t	Time10ms=0,TimeS=0,TimeM=0;		//��ʱ��



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
//---------------------------------------------------------��ˮ�ƴ���
	if (flag_1)
	{
		flag_1 = 0;
		if (flag_WLRun)
			{
			LedState++;
			LedState &= 0x07;			//�޶�״̬��Χ
			if (flag_Forward)
				{
				LedTemp =1;
				LedTemp =LedTemp << LedState;	//����״̬ȷ����Ƶ�λ��
				}
			else
				{
				LedTemp =0x80;
				LedTemp =LedTemp >> LedState;
				}
			HAL_GPIO_WritePin(GPIOC,0xff,GPIO_PIN_SET);			//�����	
			HAL_GPIO_WritePin(GPIOC,LedTemp,GPIO_PIN_RESET);	//���
			}
	}
	  
	//---------------------------------------------------------��������
	if (flag_KS)
		{
		KeyV = KEY_Scan(0);				//����ɨ��
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

            case WKUP_PRES://����BEEP
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


	//---------------------------------------------------------��ʾ����
	if (flag_DSP)
		{
		flag_DSP =0;

		smg_wei++;
		smg_wei &=0x07;
		//if(smg_wei==8) smg_wei=0;

		smg_duan =DSPBuff[smg_wei];
		LED_Write_Data(smg_num[smg_duan],smg_wei);//�������ʾ
		LED_Refresh();//ˢ����ʾ

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


//-----------------------------------------------------------�����ź�
void	Tone(uint16_t DelayV)
{
	HAL_GPIO_WritePin(BELL_GPIO_Port,BELL_Pin,GPIO_PIN_RESET);	
	HAL_Delay(DelayV);
	HAL_GPIO_WritePin(BELL_GPIO_Port,BELL_Pin,GPIO_PIN_SET);
}


//-----------------------------------------------------------TIM1�жϻص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance ==TIM1)
	{
//	  flag_1 = 1;
	CountTimer ++;
	CountTimer10ms ++;
	
	  if(CountTimer > Delayms_V)	//"Delayms_V",����ˮ��ˢ�±�־
	  {
	  	flag_1 = 1;
		CountTimer = 0;  
	  }

	  if (CountTimer10ms >9)		//10ms���ð���ɨ���־
		  {
		  CountTimer10ms =0;
		  flag_KS =1;
			
			if(flag_Forward) 
				{if(flag_WLRun) Time10ms --;
				if (Time10ms ==50)
					{
					DSPBuff[2] =17;		//��ʾ��   ��
					DSPBuff[5] =17;
					}

				if (Time10ms ==255)
					{
					Time10ms =99;

					DSPBuff[2] =16;		//��ʾ��-��
					DSPBuff[5] =16;

					TimeS --;			//����ʱ
					
					if (TimeS ==255)
						{
						TimeS =59;
						TimeM --;			//����ʱ
						if (TimeM ==255)
							{
							TimeM =59;
							}
						}
					}
				}			//����ʱ
			else 
				{if(flag_WLRun) Time10ms ++;
				if (Time10ms ==50)
					{
					DSPBuff[2] =17; 	//��ʾ��   ��
					DSPBuff[5] =17;
					}
				
				if (Time10ms >99)
					{
					Time10ms =0;
				
					DSPBuff[2] =16; 	//��ʾ��-��
					DSPBuff[5] =16;
				
					TimeS ++;			//����ʱ
					
					if (TimeS >59)
						{
						TimeS =0;
				
						TimeM ++;			//����ʱ
						if (TimeM >59)
							{
							TimeM =0;
							}
						}

					}
				}
	  }

	if (CountTimer10ms & 0x01)	//2ms����������ʾ�����־
		{
		flag_DSP =1;
		}

	}

	
}	


//-----------------------------------------------------------����������
//���ذ���ֵ
//mode��0,��֧��������;1,֧��������;
//����ֵ��//0��û���κΰ�������//KEY0_PRES��KEY0����//KEY1_PRES��KEY1����//KEY2_PRES��KEY2����//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	
	if (key_up)
	{					//�а������£�
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
	else if(KEY0==1 && KEY1==1 && KEY2==1 && WK_UP==0) key_up=1;	//����Ƿ����а�������
		return 0;

}

//-----------------------------------------------------------�������ʾ������
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

//74HC138 ����
//�����λѡ
//num:Ҫ��ʾ������ܱ�� 0-7(�� 8 �������)
void LED_Wei(uint8_t num)
{
 	HAL_GPIO_WritePin(LED_A0_GPIO_Port,LED_A0_Pin,(GPIO_PinState)(num&0x01));
	HAL_GPIO_WritePin(LED_A1_GPIO_Port,LED_A1_Pin,(GPIO_PinState)((num&0x02)>>1));
	HAL_GPIO_WritePin(LED_A2_GPIO_Port,LED_A2_Pin,(GPIO_PinState)((num&0x04)>>2));
}


//74HC595 ����
//�������ʾ
//duan:��ʾ�Ķ���
//wei:Ҫ��ʾ������ܱ�� 0-7(�� 8 �������)
void LED_Write_Data(uint8_t duan,uint8_t wei)
{
uint8_t i;
for( i=0;i<8;i++)//���Ͷ�
{
	HAL_GPIO_WritePin(LED_DS_GPIO_Port,LED_DS_Pin,(GPIO_PinState)((duan>>i)&0x01));
	HAL_GPIO_WritePin(LED_SCK_GPIO_Port,LED_SCK_Pin,GPIO_PIN_RESET);
	HAL_Delay_us(5);
	HAL_GPIO_WritePin(LED_SCK_GPIO_Port,LED_SCK_Pin,GPIO_PIN_SET);
}
 LED_Wei(wei);//��ѡ��λ
}


//74HC595 ����
//�����ˢ����ʾ
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
