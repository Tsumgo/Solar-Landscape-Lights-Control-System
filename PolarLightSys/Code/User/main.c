#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "PWM.h"
#include "main.h"
#include "Key.h"
#include "Timer.h"
#include "Serial.h"

uint8_t flag_KS = 0;		// 按键扫描标志
uint8_t uartRx_flag = 0;	// 串口接收标志
uint8_t ADC_flag = 0;		// ADC转换标志
uint8_t OLEDFlash_flag = 0; // OLED刷新标志
uint8_t OLEDflag = 0;		// OLED切换显示
uint8_t flag_test = 1;		// 检测白天黑夜
uint8_t sys_flag = 1;		// 系统开关，蓝牙控制
uint8_t LED_State = 1;		// 调试LED

uint16_t CountTimer = 0;
uint16_t CountTimer10ms = 0;
uint16_t CountTimer500ms = 0;
uint16_t CountTimer30s = 0;
uint8_t KeyV = 0; // 取得键值
uint16_t Delayms_V = 50;

double pwm_LED;			// LED亮度
double pwm_CHARGE;		// 充电电流
uint16_t chargeI = 300; // 恒流驱动电流

uint8_t VBAT_num = 0;
uint32_t VBAT_sum = 0;
uint32_t ADC_VBAT; // 电池电压采样
uint8_t VSL_num = 0;
uint32_t VSL_sum = 0;
uint32_t ADC_VSL; // 太阳能电压采样
uint8_t ILED_num = 0;
uint32_t ILED_sum = 0;
uint32_t ADC_ILED; // LED电流电压采样
uint8_t ICHARGE_num = 0;
uint32_t ICHARGE_sum = 0;
uint32_t ADC_ICHARGE; // 充电电流电压采样

uint8_t state = state_stop; // 状态指示

uint8_t ICHARGE[16];
uint8_t ILED[16];
uint8_t VBAT[16];
uint8_t VSL[16];
uint8_t TxBuf[16];
uint8_t RxByte;
uint8_t RxBuf[32];

uint8_t i; // 定义for循环的变量
char MODE[][8] = {"stop   ", "Vcharge", "Icharge", "uesLED "};

int main(void)
{
	/*模块初始化*/
	OLED_Init();  // OLED初始化
	AD_Init();	  // AD初始化
	PWM_Init();	  // PWM初始化
	Timer_Init(); // TIME初始化
	Serial_Init();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 开启GPIOB的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 将PC13引脚初始化为开漏输出

	/*显示静态字符串*/
	OLED_ShowString(1, 1, "VBAT:xxxxmV");
	OLED_ShowString(2, 1, "VSL:xxxxmV");
	OLED_ShowString(3, 1, "MODE=");

	while (1)
	{
		// ----------------------------------------- 蓝牙接收
		if (Serial_GetRxFlag() == 1)
		{
			RxByte = Serial_GetRxData(); // 获取串口接收的数据
			sys_flag = RxByte - '0';
			LED_State ^= 1;
		}
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, LED_State ^ 1);

		//---------------------------------------------------------不同状态初始化
		switch (state)
		{
		case state_stop: // 停止充电，停止放电
			pwm_CHARGE = 0;
			pwm_LED = 100;
			break;
		case state_V_charge: // 停止放电，按电压充电
			pwm_CHARGE = 100;
			pwm_LED = 100;
			break;
		case state_I_charge: // 停止放电，恒压限流充电
			pwm_LED = 100;
			break;
		case state_use_charge: // 停止充电，恒流放电
			pwm_CHARGE = 0;
			break;
		default:
			break;
		}
		pwm_LED = (pwm_LED > 60) ? pwm_LED : 60;
		PWM_LED_SetCompare2((uint16_t)pwm_LED);
		PWM_CHARGE_SetCompare1((uint16_t)pwm_CHARGE);

		//---------------------------------------------------------状态转换处理
		if (ADC_VSL <= SL_limit) // 采样到太阳能电压 <= 为电池供电的最低电压，应停止充电
		{
			if (state != state_stop && state != state_use_charge) // 如果不是停止充电或放电状态
				state = state_use_charge;						  // 切换到放电状态。
			if (ADC_VBAT >= 6400)								  // 电池电压大于6.4V
				state = state_use_charge;						  // 切换到放电状态
			else if (ADC_VBAT <= 6000)							  // 电池电压小于6V
				state = state_stop;								  // 切换到停止状态
		}
		else // 采样到太阳能电压 > 为电池供电的最低电压，应充电
		{
			` if (state == state_use_charge)									   // 如果是放电状态
				state = state_stop;												   // 停止放电
			if (ADC_ICHARGE <= 200 && ADC_VBAT >= 8200 && state == state_I_charge) // 充电电流小于200mA，电池电压大于8.2V，且为恒流充电状态时，应停止充电
				state = state_stop;
			else if (ADC_VBAT >= 8200 && state == state_V_charge) // 电池电压>=8.2V，且当前在恒压充电时，应转换为恒流充电
				state = state_I_charge;
			else if (ADC_VBAT <= 7800 && state == state_stop) // 电池电压小于7.8V，且为停止充电状态时，应恒压充电
				state = state_V_charge;
		}

		if (sys_flag == 0) // 系统总开关
		{
			state = state_stop;
		}

		//---------------------------------------------------------OLED显示模块处理
		if (OLEDFlash_flag && !OLEDflag)
		{
			OLEDFlash_flag = 0;
			OLED_ShowNum(1, 6, ADC_VBAT, 4); // 显示通道1的转换结果ADC_VBAT
			OLED_ShowNum(2, 5, ADC_VSL, 4);	 // 显示通道2的转换结果ADC_VSL
			OLED_ShowString(4, 1, MODE[state]);
		}
		if (OLEDFlash_flag && OLEDflag)
		{
			OLEDFlash_flag = 0;
			if (state == state_use_charge)
				OLED_ShowNum(2, 1, ADC_ILED, 3);
			else
				OLED_ShowNum(2, 1, 0, 3);
			OLED_ShowNum(4, 1, ADC_ICHARGE, 3);
		}

		//---------------------------------------------------------AD转换处理
		if (ADC_flag)
		{
			ADC_flag = 0; // 启动ADC转换

			// VBAT采样
			VBAT_sum += (double)AD_GetValue(ADC_Channel_1) * 3300 / 4095; // 取一秒钟内平均值
			VBAT_num++;													  // 保存采样次数

			// 电池电压校准
			ADC_VBAT = (double)VBAT_sum / VBAT_num * 8.2 / 8.12 * 147 / 47;
			if (ADC_VBAT > 7700)
				ADC_VBAT = (double)ADC_VBAT * 1.0169 - 67.588;
			else if (ADC_VBAT < 6800)
				ADC_VBAT = (double)ADC_VBAT * 1.0111 - 29.543;
			else
				ADC_VBAT += 55;
			if (state == state_V_charge || state == state_I_charge)
				ADC_VBAT -= 30;

			if (VBAT_num == 20)
			{
				VBAT_sum = 0;
				VBAT_num = 0;
			}
			// OLED显示电池电压
			VBAT[0] = ((ADC_VBAT / 1000) % 10) + 0x30;
			VBAT[1] = '.';
			VBAT[2] = ((ADC_VBAT / 100) % 10) + 0x30;
			VBAT[3] = ((ADC_VBAT / 10) % 10) + 0x30;
			VBAT[4] = ((ADC_VBAT) % 10) + 0x30;

			// ILED采样
			ILED_sum += (double)AD_GetValue(ADC_Channel_3) * 3300 / 4095;
			ILED_num++;
			ADC_ILED = ((((double)ILED_sum / ILED_num / 5.6) + 63) * 1.15954 - 33.815) > 0 ? ((((double)ILED_sum / ILED_num / 5.6) + 63) * 1.15954 - 33.815) : 0; // 线性校准
			if (ILED_num == 20)
			{
				ILED_sum = 0;
				ILED_num = 0;
			}
			ILED[0] = ((ADC_ILED / 1000) % 10) + 0x30;
			ILED[1] = ((ADC_ILED / 100) % 10) + 0x30;
			ILED[2] = ((ADC_ILED / 10) % 10) + 0x30;
			ILED[3] = ((ADC_ILED) % 10) + 0x30;

			// ICHARGE采样
			ICHARGE_sum += (double)AD_GetValue(ADC_Channel_4) * 3300 / 4095;
			ICHARGE_num++;
			ADC_ICHARGE = (((double)ICHARGE_sum / ICHARGE_num / 12) * 0.9281 + 23.96) * 1.1362 - 2.6482; // 线性校准
			if (ICHARGE_num == 20)
			{
				ICHARGE_sum = 0;
				ICHARGE_num = 0;
			}
			ICHARGE[0] = ((ADC_ICHARGE / 1000) % 10) + 0x30;
			ICHARGE[1] = ((ADC_ICHARGE / 100) % 10) + 0x30;
			ICHARGE[2] = ((ADC_ICHARGE / 10) % 10) + 0x30;
			ICHARGE[3] = ((ADC_ICHARGE) % 10) + 0x30;
			// 实现300mA恒流控制
			if (state == state_use_charge)
			{
				if (ADC_ILED > chargeI + 15)
					pwm_LED += 0.3;
				else if (ADC_ILED > chargeI)
					pwm_LED += 0.03;
				else if (ADC_ILED > chargeI - 15)
					pwm_LED -= 0.03;
				else
					pwm_LED -= 0.3;
			}
		}

		//---------------------------------------------------------按键处理
		if (flag_KS)
		{
			KeyV = Key_GetNum();
			switch (KeyV)
			{
			case S1_PRES: // S1增加亮度
				if (state == state_use_charge)
					chargeI = (chargeI > 300) ? 325 : chargeI + 25;
				break;

			case S2_PRES: // S2降低亮度
				if (state == state_use_charge)
					chargeI = (chargeI < 25) ? 0 : chargeI - 25;
				break;

			case S3_PRES: // S3手动切换模式
				state++;
				state %= 4;
				break;

			case S4_PRES: // S4切换显示
				OLEDflag++;
				OLEDflag %= 2;
				OLED_Clear();
				if (!OLEDflag)
				{
					OLED_ShowString(1, 1, "VBAT:xxxxmV");
					OLED_ShowString(2, 1, "VSL:xxxxmV");
					OLED_ShowString(3, 1, "MODE=");
				}
				else
				{
					OLED_ShowString(1, 1, "ILED:");
					OLED_ShowString(2, 1, "xxxmA");
					OLED_ShowString(3, 1, "ICHARGE:");
					OLED_ShowString(4, 1, "xxxmA");
				}
				break;

			default:
				break;
			}
		}
		//---------------------------------------------------------太阳能电池电压检测
		if (flag_test)
		{
			flag_test = 0;

			PWM_CHARGE_SetCompare1(100);
			Delay_ms(50);
			// VSL采样
			ADC_VSL = (double)AD_GetValue(ADC_Channel_2) * 3300 / 4095 * 147 / 47;

			VSL[0] = ((ADC_VSL / 1000) % 10) + 0x30;
			VSL[1] = '.';
			VSL[2] = ((ADC_VSL / 100) % 10) + 0x30;
			VSL[3] = ((ADC_VSL / 10) % 10) + 0x30;
			VSL[4] = ((ADC_VSL) % 10) + 0x30;
		}
	}
}

//---------------------------------------------------------TIM2中断回调函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		CountTimer++;
		CountTimer10ms++;
		CountTimer500ms++;
		CountTimer30s++;

		if (CountTimer > Delayms_V) //"Delayms_V", ADC采样标志
		{
			CountTimer = 0;
			ADC_flag = 1;
		}

		if (CountTimer10ms > 9) // 10ms， 按键采样标志
		{
			CountTimer10ms = 0;
			flag_KS = 1;
		}

		if (CountTimer500ms > 499) // 500ms，OLED刷新标志
		{
			CountTimer500ms = 0;
			OLEDFlash_flag = 1;
			// 恒压限流充电
			if (state == state_I_charge)
			{
				if (ADC_ICHARGE > 200 && ADC_VBAT > 8220)
					pwm_CHARGE -= 0.1;
			}
		}

		if (CountTimer30s > 9999) // 10s， 检测solar电压
		{
			CountTimer30s = 0;
			flag_test = 1;
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
