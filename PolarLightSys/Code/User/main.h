
#ifndef __MAIN_H
#define __MAIN_H

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ADC_VBAT_Pin GPIO_PIN_1
#define ADC_VBAT_GPIO_Port GPIOA
#define ADC_VSL_Pin GPIO_PIN_2
#define ADC_VSL_GPIO_Port GPIOA
#define ADC_ILED_Pin GPIO_PIN_3
#define ADC_ILED_GPIO_Port GPIOA
#define ADC_ICHARGE_Pin GPIO_PIN_4
#define ADC_ICHARGE_GPIO_Port GPIOA
#define S1_Pin GPIO_PIN_0
#define S1_GPIO_Port GPIOB
#define S2_Pin GPIO_PIN_1
#define S2_GPIO_Port GPIOB
#define S3_Pin GPIO_PIN_10
#define S3_GPIO_Port GPIOB
#define S4_Pin GPIO_PIN_11
#define S4_GPIO_Port GPIOB

#define S1        HAL_GPIO_ReadPin(S1_GPIO_Port,S1_Pin)  //S1����PB0
#define S2        HAL_GPIO_ReadPin(S2_GPIO_Port,S2_Pin)  //S2����PB1
#define S3        HAL_GPIO_ReadPin(S3_GPIO_Port,S3_Pin)  //S3����PB10
#define S4        HAL_GPIO_ReadPin(S4_GPIO_Port,S4_Pin)  //S4����PB11

#define S1_PRES	1	//S1  
#define S2_PRES	2	//S2 
#define S3_PRES	3	//S3
#define S4_PRES	4	//S4 

#define state_stop    		0 //����ģʽ
#define state_V_charge  	1 //��ѹ���ģʽ
#define state_I_charge  	2 //�������ģʽ
#define state_use_charge  3 //����ģʽ


#define SL_limit    2000   //�����ҹ�ֽ���

#endif /* __MAIN_H */
