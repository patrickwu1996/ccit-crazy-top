#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//LED�X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/2
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��l��PB5�MPE5����X�f.�èϯ�o��Ӥf������		    
//LED IO��l��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //�ϯ�PB,PE�ݤf����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_5); 						 //PE.5 ��X�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_6);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_6); 						 //PE.5 ��X�� 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_7); 						 //PE.5 ��X�� 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_8);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_8); 						 //PE.5 ��X�� 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_9); 						 //PE.5 ��X�� 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_1); 						 //PE.5 ��X�� 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PB.5 �ݤf�t�m
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�f�t�׬�50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�ھڳ]�w�Ѽƪ�l��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PB.5 ��X��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PE.5 �ݤf�t�m, ������X
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������X �AIO�f�t�׬�50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_2); 						 //PE.5 ��X�� 
}
 
