/* Function List:
*
* 1. void delay(unsigned int z) -- 延时函数,毫秒
* 2. void IIC_Start() -- 开启I2C总线
* 3. void IIC_Stop() -- 关闭I2C总线
* 4. void Write_IIC_Byte(unsigned char IIC_Byte) -- 通过I2C总线写一个byte的数据
* 5. void OLED_WrDat(unsigned char dat) -- 向OLED屏写数据
* 6. void OLED_WrCmd(unsigned char cmd) -- 向OLED屏写命令
* 7. void OLED_Set_Pos(unsigned char x, unsigned char y) -- 设置显示坐标
* 8. void OLED_Fill(unsigned char bmp_dat) -- 全屏显示(显示BMP图片时才会用到此功能)
* 9. void OLED_CLS(void) -- 复位/清屏
* 10. void OLED_Init(void) -- OLED屏初始化程序，此函数应在操作屏幕之前最先调用
* 11. void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]) -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
* 12. void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]) -- 8x16点整，用于显示ASCII码，非常清晰
* 13.void OLED_P16x16Ch(unsigned char x, y, N) -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
* 14.void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
*
* History: none;
*
*************************************************************************************/

#ifndef _OLED12864_H_
#define _OLED12864_H_
#include "main.h"
#include "gpio.h"
#include "codetab.h"


#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/*********************OLED驱动程序用的延时程序************************************/
void delay(unsigned int z);
void IIC_GPIO_Init(void);
void OLED_WrDat(unsigned char IIC_Data);
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_dat);
void OLED_Clear(void);
void OLED_Init(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ColorTurn(unsigned char i);
void OLED_DisplayTurn(unsigned char i);
void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
void OLED_P8x16Char(unsigned char x, unsigned char y,unsigned char ch);
void OLED_P8x16NUM(unsigned char x, unsigned char y,unsigned char NUM);
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const char BMP[]);
void OLED_ShowStr_HZ(unsigned char x, unsigned char y, unsigned char *str);
void OLED_P16X32Str(unsigned char x, unsigned char y,unsigned char *str);

#endif
