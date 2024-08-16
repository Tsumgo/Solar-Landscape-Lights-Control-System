/************************************************************************************
* Description:
* 1. 128*64����OLEDģ�鹦��
* 2. �ֱ��ɴ�������еġ�ȡ�����������ó�;
* 3. 16*16�ĵ���ȡ�ַ�ʽ -- ����������ʽ���������
*/

#ifndef _CODETAB_H_
#define _CODETAB_H_

/**********16*16�ĺ��ֵ���******************************/
extern const char  F16x16[128*32];	//��С��128�����֣�ÿ������32byte��16x16=16x2x8=32byte��

/**********6*8�ĵ���************************************/
extern const char  F6x8[92][6];	//92���ַ�

/**********8*16�ĵ���************************************/
extern const char  F8X16[128*16];//�ܹ�֧��128��ASCII�ַ�,ÿ���ַ�16byte��16x8=16byte��
extern const char  F16X32[128*64];//�ܹ�֧��10�������ַ�,ÿ���ַ�64byte��32x16=64byte��

/*********8*16�ĵ���************************************/
extern const char  BMP1[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  BMP2[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  qiangge[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  guoqi[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  dangqi[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  wukong[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  peiqi[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  wukongpeiqi[64*16];	//64�У�ÿ��16��byte��16x8=128��
extern const char  peiqi32X32[];
typedef struct		// ������ģ���ݽṹ
{
	char Index[2];	// ������������
	char Msk[32];		// ����������
} typFNT_GB16;
extern const typFNT_GB16 GB_16[];


#endif
