/*
* 
* Description:128*64点整OLED模块驱动
*
* Others: none;
*
* Function List:
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

#include "main.h"
#include "i2c.h"
#include "OLED12864.h"
#include "codetab.h"

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/*********************OLED驱动程序用的延时程序************************************/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	HAL_I2C_Mem_Write(&hi2c2,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&IIC_Data,1,100);
}
/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	HAL_I2C_Mem_Write(&hi2c2,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&IIC_Command,1,100);
}
/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x00);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(bmp_dat);
	}
}
/*********************OLED清屏************************************/
void OLED_Clear(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(0);
	}
}
/*********************OLED开屏幕************************************/
void OLED_ON(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X14);  //开启电荷泵
	OLED_WrCmd(0XAF);  //OLED唤醒
}
/*********************OLED关屏幕************************************/
void OLED_OFF(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X10);  //关闭电荷泵
	OLED_WrCmd(0XAE);  //OLED休眠
}
/*********************OLED反色显示************************************/
void OLED_ColorTurn(unsigned char i)
{
	if(i==0)
	{
		OLED_WrCmd(0xA6);//正常显示
	}
	else if(i==1)
	{
		OLED_WrCmd(0xA7);//反色显示
	}
}
/*********************OLED屏幕旋转180度************************************/
void OLED_DisplayTurn(unsigned char i)
{
	if(i==0)
	{
		OLED_WrCmd(0xC8);//正常显示
		OLED_WrCmd(0xA1);
	}
	else if(i==1)
	{
		OLED_WrCmd(0xC0);//反转显示
		OLED_WrCmd(0xA0);
	}
}
/*********************OLED初始化************************************/
void OLED_Init(void)
{
	delay(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness

	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
} 


/*****功能描述：显示8*16的一位ASCII字符，显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Char(unsigned char x, unsigned char y,unsigned char ch)
{
	unsigned char i=0;	
	unsigned char c =ch-32;
	OLED_Set_Pos(x,y);
	for(i=0;i<8;i++)
		OLED_WrDat(F8X16[16*c+i]);
	OLED_Set_Pos(x,y+1);
	for(i=0;i<8;i++)
		OLED_WrDat(F8X16[16*c+i+8]);
}

/****功能描述：显示16*16点阵汉字或8*16 ASCII字符，显示的坐标（x,y），
               x范围0~119，y为页范围0～7**********************/
void OLED_ShowStr_HZ(unsigned char x, unsigned char y, unsigned char *str)
{
	unsigned int k=0;
	unsigned char j;
	while(*str)		//数据未结束
	{ 
		if(*str<128)			//ASCII字符
		{
			if(x>120){x=0;y+=2;}		//128-8=120，一行的最后，不够一个字符宽度，切换到下一行，从第一列开始显示
			OLED_P8x16Char(x,y,*str);
			x=x+8;
			str++;
		}
		else						//中文汉字
		{
			if(x>112){x=0;y+=2;}		//128-16=112，一行的最后，不够一个汉字宽度，切换到下一行，从第一列开始显示
			for(k=0;k<3550;k++) 	//遍历所有汉字库，逐个对比
			{
				if((GB_16[k].Index[0]==*(str))&&(GB_16[k].Index[1]==*(str+1)))	//如果对比成功
				{ 	
					OLED_Set_Pos(x,y);		//设置x坐标，设置页地址，显示汉字的上半部分
					for(j=0;j<16;j++)
					{
						OLED_WrDat(GB_16[k].Msk[j]);
					}
					OLED_Set_Pos(x,y+1);	//设置x坐标，页加1，显示汉字下半部分
					for(j=0;j<16;j++)
					{
						OLED_WrDat(GB_16[k].Msk[j+16]);
					} 	  	
					x=x+16; 
					break;  //查找到对应点阵字库立即退出，继续找下一个汉字
				}				  	
			}						
			str+=2;			//每个汉字对应两个字符，所以是指针加2
		}
	} 	  	
}
/****功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************
*****sizex是图片的宽度范围1～128，sizey为图片的高度范围1～64*****************
*/
void Draw_BMP(unsigned char x, unsigned char y,unsigned char sizex, unsigned char sizey,const char BMP[])
{
	unsigned int j=0;
	unsigned char dx,dy;

	if(sizey % 8==0) 			//将图片的高度转换成页数，每8行是一页
		sizey = sizey/8;      
	else 
		sizey = sizey/8+1;	//如果有余数，页就多加1.就是多余的也要用一页来存
	for(dy=0;dy<sizey;dy++)	//逐页去写
	{
		OLED_Set_Pos(x,y+dy);	//
		for(dx=0;dx<sizex;dx++)
		{      
			OLED_WrDat(BMP[j++]);
		}
	}
}


/***************功能描述：显示6*8一组标准ASCII字符串 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>122){x=0;y++;}//128-6=122，一行的最后，不够一个字符宽度，切换到下一行，从第一列开始显示
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
			OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}

///*******************功能描述：显示8*16的一位数字，显示的坐标（x,y），y为页范围0～7****************/
//void OLED_P8x16NUM(unsigned char x, unsigned char y,unsigned char NUM)
//{
//	unsigned char i=0;	
//	OLED_Set_Pos(x,y);
//	for(i=0;i<8;i++)
//		OLED_WrDat(F8X16[16*16+NUM*16+i]);
//	OLED_Set_Pos(x,y+1);
//	for(i=0;i<8;i++)
//		OLED_WrDat(F8X16[16*16+NUM*16+i+8]);
//}

/****功能描述：显示16*32的ASCII字符串，显示的坐标（x,y），y为页范围0～7****************/
void OLED_P16X32Str(unsigned char x, unsigned char y,unsigned char *str)
{
	unsigned char i=0;	
	unsigned char c;
	while(*str)
	{
		c =*str-32;
		OLED_Set_Pos(x,y);
		for(i=0;i<16;i++)
			OLED_WrDat(F16X32[c*64+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<16;i++)
			OLED_WrDat(F16X32[c*64+i+16]);
		OLED_Set_Pos(x,y+2);
		for(i=0;i<16;i++)
			OLED_WrDat(F16X32[c*64+i+32]);
		OLED_Set_Pos(x,y+3);
		for(i=0;i<16;i++)
			OLED_WrDat(F16X32[c*64+i+48]);
		x+=16;
		str++;
	}
}
