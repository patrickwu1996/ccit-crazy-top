#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32�}�o�O
//�ϥ�SysTick�����q�p�ƼҦ��冀��i��޲z�]�A�XSTM32F10x�t�C�^
//�]�Adelay_us,delay_ms
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2010/1/1
//�����GV1.8
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved
//********************************************************************************
//V1.2�קﻡ��
//�ץ��F���_���եΥX�{���`�������~
//����ɤ��ǽT,�ĥ�do while���c!
//V1.3�קﻡ��
//�W�[�F��UCOSII���ɪ����.
//�p�G�ϥ�ucosII,delay_init�|�۰ʳ]�mSYSTICK����,�Ϥ��Pucos��TICKS_PER_SEC����.
//delay_ms�Mdelay_us�]�i��F�w��ucos����y.
//delay_us�i�H�bucos�U�ϥ�,�ӥB�ǽT�׫ܰ�,�󭫭n���O�S���e���B�~���w�ɾ�.
//delay_ms�bucos�U,�i�H��OSTimeDly�ӥ�,�b���Ұ�ucos��,���ĥ�delay_us��{,�q�ӷǽT����
//�i�H�ΨӪ�l�ƥ~�],�b�ҰʤFucos����delay_ms�ھک��ɪ����u,���OSTimeDly��{�Ϊ�delay_us��{.
//V1.4�קﻡ�� 20110929
//�ק�F�ϥ�ucos,���Oucos���Ұʪ��ɭ�,delay_ms�����_�L�k�T����bug.
//V1.5�קﻡ�� 20120902
//�bdelay_us�[�Jucos�W��A����ѩ�ucos���_delay_us������A�i��ɭP�����ɤ��ǡC
//V1.6�קﻡ�� 20150109
//�bdelay_ms�[�JOSLockNesting�P�_�C
//V1.7�קﻡ�� 20150319
//�ק�OS����覡,�H������NOS(������UCOSII�MUCOSIII,�z�פW���NOS���i�H���)
//�K�[:delay_osrunning/delay_ostickspersec/delay_osintnesting�T�ӧ��w�q
//�K�[:delay_osschedlock/delay_osschedunlock/delay_ostimedly�T�Ө��
//V1.8�קﻡ�� 20150519
//�ץ�UCOSIII����ɪ�2��bug�G
//delay_tickspersec�אּ�Gdelay_ostickspersec
//delay_intnesting�אּ�Gdelay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 
void delay_init1(void);
void delay_ms1(u16 nms);
void delay_us1(u32 nus);

#endif





























