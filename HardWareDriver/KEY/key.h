#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//�����X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/3
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//Ū������0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//Ū������1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//Ū������2 
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//Ū������3(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0���U
#define KEY1_PRES	2	//KEY1���U
#define KEY2_PRES	3	//KEY2���U
#define WKUP_PRES   4	//KEY_UP���U(�YWK_UP/KEY_UP)


void KEY_Init(void);//IO��l��
u8 KEY_Scan(u8);  	//���䱽�y���					    
#endif
