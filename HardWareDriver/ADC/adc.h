#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//ADC �N�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/7
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
