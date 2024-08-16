/************************************************************************************
* Description:
* 1. 128*64点整OLED模块功能
* 2. 字表由打包资料中的“取字软件”计算得出;
* 3. 16*16的点阵取字方式 -- 共阴、列行式、逆向输出
*/

#ifndef _CODETAB_H_
#define _CODETAB_H_

/**********16*16的汉字点阵******************************/
extern const char  F16x16[128*32];	//大小是128个汉字，每个汉字32byte（16x16=16x2x8=32byte）

/**********6*8的点阵************************************/
extern const char  F6x8[92][6];	//92个字符

/**********8*16的点阵************************************/
extern const char  F8X16[128*16];//总共支持128个ASCII字符,每个字符16byte（16x8=16byte）
extern const char  F16X32[128*64];//总共支持10个数字字符,每个字符64byte（32x16=64byte）

/*********8*16的点阵************************************/
extern const char  BMP1[64*16];	//64行，每行16个byte（16x8=128）
extern const char  BMP2[64*16];	//64行，每行16个byte（16x8=128）
extern const char  qiangge[64*16];	//64行，每行16个byte（16x8=128）
extern const char  guoqi[64*16];	//64行，每行16个byte（16x8=128）
extern const char  dangqi[64*16];	//64行，每行16个byte（16x8=128）
extern const char  wukong[64*16];	//64行，每行16个byte（16x8=128）
extern const char  peiqi[64*16];	//64行，每行16个byte（16x8=128）
extern const char  wukongpeiqi[64*16];	//64行，每行16个byte（16x8=128）
extern const char  peiqi32X32[];
typedef struct		// 汉字字模数据结构
{
	char Index[2];	// 汉字内码索引
	char Msk[32];		// 点阵码数据
} typFNT_GB16;
extern const typFNT_GB16 GB_16[];


#endif
