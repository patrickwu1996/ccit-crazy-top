#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//LED驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5為輸出口.並使能這兩個口的時鐘		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口時鐘
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_5); 						 //PE.5 輸出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_6);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_6); 						 //PE.5 輸出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_7); 						 //PE.5 輸出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_8);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_8); 						 //PE.5 輸出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_9); 						 //PE.5 輸出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_1); 						 //PE.5 輸出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度為50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根據設定參數初始化GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PB.5 輸出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PE.5 端口配置, 推輓輸出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推輓輸出 ，IO口速度為50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_2); 						 //PE.5 輸出高 
}
 
