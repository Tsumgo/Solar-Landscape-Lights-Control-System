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
V09��OLED��ʾ��jinxd��230707
	1��OLED��ʾ�ɼ���ѹ��
	2��ÿ��ɼ�һ�Σ�
	3��OLED��ˢ�±Ƚϻ�ʱ�䣡

V08��ADCת��-��λ��RV1��ѹֵ��jinxd��230706
	1������AD�󣬲��ò�ѯ��ʽ��ȡADֵ����ת���ɵ�ѹ��
	2��ͨ�����ڷ��͵�ѹֵ��ADC value =1.415V
	3���������ʾ��ѹֵ��

V07������ͨ��-�жϷ�ʽ����һ֡���ݣ�jinxd��230705
	1��ʹ��Uart1���˿�PA9��PA10��
	2��ATָ������������
		AT��ͨ�Ų��ԣ�
		AT+RUN=0/1����ˮ�����п��ƣ�
		AT+GAP=20/90���ٶȲ������䡣

V06������ͨ�ţ�jinxd��230705
	1��ʹ��Uart1���˿�PA9��PA10��
	2����ѯ��ʽ���͡�Hello world!\n\r��

V05��PWM�����jinxd��230704
	1��TIM2-CH2���PWM�źţ�PA1��
	2������20ms������0.5-2.5ms��ͨ��KEY1��KEYUP���ڣ�
	3���������ʾ��������λΪ0.1ms��
	3����ֱ�����������

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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "OLED12864.h"
#include "codetab.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

uint8_t KEY_Scan(uint8_t mode);
void Tone(uint16_t DelayV);
void LED_Write_Data(uint8_t duan, uint8_t wei);
void LED_Refresh(void);
uint8_t Compare_string(char *s);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t flag_1 = 0;			// ��ˮ�ƶ�ʱ��־
uint8_t flag_KS = 0;		// ����ɨ���־
uint8_t flag_WLRun = 0;		// ��ˮ�����б�־
uint8_t flag_Forward = 0;	// ��ˮ�Ʒ����־
uint8_t flag_DSP = 0;		// �������ʾ������ÿ5ms����
uint8_t uartRx_flag = 0;	// ���ڽ��ձ�־
uint8_t RxTimeOut_flag = 0; // Uart���ճ�ʱ��־
uint8_t ADC_flag = 0;		// ADCת����־
uint8_t OLEDFlash_flag = 0; // OLEDˢ�±�־

uint16_t CountTimer = 0;
uint16_t CountTimer10ms = 0;
uint8_t KeyV = 0; // ȡ�ü�ֵ
uint16_t Delayms_V = 50;
uint8_t LedState = 0;
uint8_t LedTemp = 0;

uint8_t smg_wei = 0;  // �����λѡ
uint8_t smg_duan = 0; // ����ܶ�ѡ
uint16_t t = 0;
uint8_t DSPBuff[8] = {16, 16, 16, 16, 16, 6, 7, 8}; // ������Դ�
// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,ȫ��
uint8_t smg_num[] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe,
					 0xf6, 0xee, 0x3e, 0x9c, 0x7a, 0x9e, 0x8e, 0x02, 0x00};
uint8_t Time10ms = 0, TimeS = 0, TimeM = 0; // ��ʱ��

uint16_t pwm_val = 0;

uint8_t TxBuf[16];
uint8_t RxByte;
uint8_t RxBuf[32];
uint8_t RxCount = 0;
uint8_t RxLen = 0;
uint8_t RxTimeOutCount = 5;

uint16_t ADCValue;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	//	uint8_t	TxBuf[] ="Usart Test\r\n";
	uint8_t i;

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
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim1);

	HAL_UART_Receive_IT(&huart1, &RxByte, 1);

	OLED_Init();
	//	OLED_P8x16Char(0,0,'k');
	OLED_P16X32Str(0, 0, "Voltag=");
	//	OLED_P16X32Str(0,0,"��");
	OLED_P16X32Str(0, 4, "  xxxxmV");
	//	HAL_Delay(2000);
	OLED_DisplayTurn(0);
	//	HAL_Delay(2000);
	//	Draw_BMP(30,0,32,32,peiqi32X32);
	//	HAL_Delay(2000);
	OLED_ColorTurn(0);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (OLEDFlash_flag)
		{
			OLEDFlash_flag = 0;
			// sprintf(TxBuf,"%02d:%02d:%02d",time[0],time[1],time[2]++);
			OLED_P16X32Str(32, 4, (uint8_t *)TxBuf);
			// OLED_P6x8Str(0,7,(uint8_t *)TxBuf);
		}
		//---------------------------------------------------------ADת����������
		if (ADC_flag)
		{
			ADC_flag = 0;
			HAL_ADC_Start(&hadc1);				   // ����ADCת��
			HAL_ADC_PollForConversion(&hadc1, 10); // �ȴ�ת�����
			ADCValue = HAL_ADC_GetValue(&hadc1);   // ��ȡת��ֵ
			// printf("ADC value = %4d\r\n",ADCValue);		//��ӡ���
			HAL_UART_Transmit(&huart1, (uint8_t *)"ADC value =", 11, 11);

			ADCValue = ADCValue * 3300 / 4095; // ADֵת��Ϊ��ѹֵ

			TxBuf[0] = ((ADCValue / 1000) % 10) + 0x30; // ���ڷ��͵�ѹֵ
			TxBuf[1] = '.';
			TxBuf[2] = ((ADCValue / 100) % 10) + 0x30;
			TxBuf[3] = ((ADCValue / 10) % 10) + 0x30;
			TxBuf[4] = ((ADCValue) % 10) + 0x30;
			HAL_UART_Transmit(&huart1, TxBuf, 5, 5);
			HAL_UART_Transmit(&huart1, (uint8_t *)"V\r\n", 3, 3);

			DSPBuff[4] = (ADCValue / 1000) % 10; // �������ʾ��ѹֵ
			DSPBuff[5] = (ADCValue / 100) % 10;
			DSPBuff[6] = (ADCValue / 10) % 10;
			DSPBuff[7] = ADCValue % 10;
		}

		//---------------------------------------------------------���ڽ��մ���
		if (uartRx_flag)
		{
			uartRx_flag = 0;
			if ((Compare_string("AT")) && (RxLen == 2)) // ͨ�Ų���
			{
				HAL_UART_Transmit(&huart1, (uint8_t *)"OK\r\n", 4, 4);
				// printf("AT");
			}
			else if (Compare_string("AT+RUN=")) // ���п���
				flag_WLRun = RxBuf[7] - 0x30;

			else if (Compare_string("AT+GAP=")) // ��������
				Delayms_V = (RxBuf[7] - 0x30) * 10 + (RxBuf[8] - 0x30);

			Tone(50);

			for (i = 0; i < 32; ++i) // ���������
			{
				RxBuf[i] = 0;
			}
		}

		//---------------------------------------------------------��ˮ�ƴ���
		if (flag_1)
		{
			flag_1 = 0;
			if (flag_WLRun)
			{
				LedState++;
				LedState &= 0x07; // �޶�״̬��Χ
				if (flag_Forward)
				{
					LedTemp = 1;
					LedTemp = LedTemp << LedState; // ����״̬ȷ����Ƶ�λ��
				}
				else
				{
					LedTemp = 0x80;
					LedTemp = LedTemp >> LedState;
				}
				HAL_GPIO_WritePin(GPIOC, 0xff, GPIO_PIN_SET);	   // �����
				HAL_GPIO_WritePin(GPIOC, LedTemp, GPIO_PIN_RESET); // ���
			}
		}

		//---------------------------------------------------------��������
		if (flag_KS)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 + pwm_val);

			// HAL_UART_Transmit(&huart1,(uint8_t *)"Hello world!\r\n", 14, 14);	//��ѯ��ʽ����
			// HAL_UART_Transmit(&huart1,TxBuf,12, 10);

			// HAL_UART_Transmit_IT(&huart1,(uint8_t *)"Hello world!\r\n", 14);
			// HAL_Delay(5);
			// HAL_UART_Transmit_IT(&huart1,TxBuf,12);

			KeyV = KEY_Scan(0); // ����ɨ��
			switch (KeyV)
			{
			case KEY0_PRES: // KEY0
				Tone(50);
				flag_Forward++;
				flag_Forward &= 0x01;
				pwm_val = 0;
				break;

			case KEY1_PRES: // KEY1
				Tone(50);
				if (Delayms_V < 200)
				{
					Delayms_V += 5;
				}

				if (pwm_val >= 100) // ��СPWM����
				{
					pwm_val -= 100;
				}
				break;

			case WKUP_PRES: // ����BEEP
				Tone(50);
				if (Delayms_V > 5)
				{
					Delayms_V -= 5;
				}

				if (pwm_val <= 1900) // ����PWM����
				{
					pwm_val += 100;
				}
				break;

			case KEY2_PRES: // KEY_UP
				Tone(50);
				flag_WLRun++;
				flag_WLRun &= 0x01;
				pwm_val = 2000;
				break;

			default:
				break;
			}
		}

		//---------------------------------------------------------��ʾ����
		if (flag_DSP)
		{
			flag_DSP = 0;

			smg_wei++;
			smg_wei &= 0x07;
			// if(smg_wei==8) smg_wei=0;

			smg_duan = DSPBuff[smg_wei];
			LED_Write_Data(smg_num[smg_duan], smg_wei); // �������ʾ
			LED_Refresh();								// ˢ����ʾ

			if (smg_wei == 7)
			{
				/*			DSPBuff[0] =TimeM /10;
							DSPBuff[1] =TimeM %10;

							DSPBuff[3] =TimeS /10;
							DSPBuff[4] =TimeS %10;

							DSPBuff[6] =Time10ms /10;
							DSPBuff[7] =Time10ms %10;
				*/

				/*			DSPBuff[7] =((pwm_val+500)/100) %10;
							DSPBuff[6] =((pwm_val+500)/1000) %10;
							DSPBuff[5] =((pwm_val+500)/10000) %10;
				*/
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
	return ch;
}

uint8_t Compare_string(char *s)
{
	uint8_t i = 0;
	//	Equ_flag =1;
	while (*s)
	{
		if (RxBuf[i++] != *s++)
		{
			// Equ_flag =0;
			return 0;
		}
	}
	return 1;
}

//---------------------------------------------------------�жϴ�������
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		// uartRx_flag =1;
		RxBuf[RxCount] = RxByte;
		RxCount++;
		RxTimeOutCount = 5;
		/*
				if (RxTimeOut_flag)
					{
					RxLen =RxCount;
					}
		*/
		HAL_UART_Receive_IT(&huart1, &RxByte, 1);
	}
}

//-----------------------------------------------------------�����ź�
void Tone(uint16_t DelayV)
{
	HAL_GPIO_WritePin(BELL_GPIO_Port, BELL_Pin, GPIO_PIN_RESET);
	HAL_Delay(DelayV);
	HAL_GPIO_WritePin(BELL_GPIO_Port, BELL_Pin, GPIO_PIN_SET);
}

//-----------------------------------------------------------TIM1�жϻص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{

		if (RxTimeOutCount != 0) // �ж�Rx��ʱ��
		{
			RxTimeOutCount--;
			if (RxTimeOutCount == 0)
			{
				uartRx_flag = 1;
				RxLen = RxCount;
				RxCount = 0;
			}
		}

		//	  flag_1 = 1;
		CountTimer++;
		CountTimer10ms++;

		if (CountTimer > Delayms_V) //"Delayms_V"
		{
			flag_1 = 1;
			CountTimer = 0;
		}

		if (CountTimer10ms > 9) // 10ms���ð���ɨ���־
		{
			CountTimer10ms = 0;
			flag_KS = 1;

			if (flag_Forward)
			{
				Time10ms--;
				if (Time10ms == 50)
				{
					ADC_flag = 1; // ADC��־
								  // DSPBuff[2] =17;		//��ʾ��   ��
								  // DSPBuff[5] =17;
				}

				if (Time10ms == 255)
				{
					Time10ms = 99;

					// DSPBuff[2] =16;		//��ʾ��-��
					// DSPBuff[5] =16;

					TimeS--;			// �����ʱ
					OLEDFlash_flag = 1; // OLEDÿ��ˢ��һ��

					if (TimeS == 255)
					{
						TimeS = 59;
						TimeM--; // �����ʱ
						if (TimeM == 255)
						{
							TimeM = 59;
						}
					}
				}
			} // �����ʱ
			else
			{
				Time10ms++;
				if (Time10ms == 50) // 500ms
				{
					ADC_flag = 1; // ADC��־
								  // DSPBuff[2] =17; 	//��ʾ��   ��
								  // DSPBuff[5] =17;
				}

				if (Time10ms > 99)
				{
					Time10ms = 0;

					// DSPBuff[2] =16; 	//��ʾ��-��
					// DSPBuff[5] =16;

					TimeS++;			// �����ʱ
					OLEDFlash_flag = 1; // OLEDÿ��ˢ��һ��
					if (TimeS > 59)
					{
						TimeS = 0;

						TimeM++; // �����ʱ
						if (TimeM > 59)
						{
							TimeM = 0;
						}
					}
				}
			}
		}

		if (CountTimer10ms & 0x01) // 2ms����������ʾ������־
		{
			flag_DSP = 1;
		}
	}
}

//-----------------------------------------------------------������������
// ���ذ���ֵ
// mode��0,��֧��������;1,֧��������;
// ����ֵ��//0��û���κΰ�������//KEY0_PRES��KEY0����//KEY1_PRES��KEY1����//KEY2_PRES��KEY2����//WKUP_PRES��WK_UP����
// ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1; // �������ɿ���־
	if (mode)
		key_up = 1; // ֧������

	if (key_up)
	{ // �а������£�
		if (KEY0 == 0)
		{
			key_up = 0;
			return KEY0_PRES;
		}
		else if (KEY1 == 0)
		{
			key_up = 0;
			return KEY1_PRES;
		}
		else if (KEY2 == 0)
		{
			key_up = 0;
			return KEY2_PRES;
		}
		else if (WK_UP == 1)
		{
			key_up = 0;
			return WKUP_PRES;
		}
	}
	else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0)
		key_up = 1; // ����Ƿ����а�������
	return 0;
}

//-----------------------------------------------------------�������ʾ��������
void HAL_Delay_us(uint16_t us)
{
	uint16_t Temp;

	if (us--)
	{
		for (Temp = 0; Temp < 72; ++Temp)
		{
		}
	}
}

// 74HC138 ����
// �����λѡ
// num:Ҫ��ʾ������ܱ�� 0-7(�� 8 �������)
void LED_Wei(uint8_t num)
{
	HAL_GPIO_WritePin(LED_A0_GPIO_Port, LED_A0_Pin, (GPIO_PinState)(num & 0x01));
	HAL_GPIO_WritePin(LED_A1_GPIO_Port, LED_A1_Pin, (GPIO_PinState)((num & 0x02) >> 1));
	HAL_GPIO_WritePin(LED_A2_GPIO_Port, LED_A2_Pin, (GPIO_PinState)((num & 0x04) >> 2));
}

// 74HC595 ����
// �������ʾ
// duan:��ʾ�Ķ���
// wei:Ҫ��ʾ������ܱ�� 0-7(�� 8 �������)
void LED_Write_Data(uint8_t duan, uint8_t wei)
{
	uint8_t i;
	for (i = 0; i < 8; i++) // ���Ͷ�
	{
		HAL_GPIO_WritePin(LED_DS_GPIO_Port, LED_DS_Pin, (GPIO_PinState)((duan >> i) & 0x01));
		HAL_GPIO_WritePin(LED_SCK_GPIO_Port, LED_SCK_Pin, GPIO_PIN_RESET);
		HAL_Delay_us(5);
		HAL_GPIO_WritePin(LED_SCK_GPIO_Port, LED_SCK_Pin, GPIO_PIN_SET);
	}
	LED_Wei(wei); // ��ѡ��λ
}

// 74HC595 ����
// �����ˢ����ʾ
void LED_Refresh(void)
{
	HAL_GPIO_WritePin(LED_LCLK_GPIO_Port, LED_LCLK_Pin, GPIO_PIN_SET);
	HAL_Delay_us(5);
	HAL_GPIO_WritePin(LED_LCLK_GPIO_Port, LED_LCLK_Pin, GPIO_PIN_RESET);
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

#ifdef USE_FULL_ASSERT
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
