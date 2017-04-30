#include "timer.h"
#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//定時器 驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函數。
//2,增加LED0_PWM_VAL宏定義，控制TIM3_CH2脈寬									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定時器3中斷初始化
//這裡時鐘選擇為APB1的2倍，而APB1為36M
//arr：自動重裝值。
//psc：時鐘預分頻數
//這裡使用的是定時器3!
/*void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //時鐘使能

	TIM_TimeBaseStructure.TIM_Period = arr; //設置在下一個更新事件裝入活動的自動重裝載寄存器週期的值	 計數到5000為500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //設置用來作為TIMx時鐘頻率除數的預分頻值  10Khz的計數頻率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //設置時鐘分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上計數模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根據TIM_TimeBaseInitStruct中指定的參數初始化TIMx的時間基數單位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中斷,允許更新中斷

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中斷
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占優先級0級
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //從優先級3級
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根據NVIC_InitStruct中指定的參數初始化外設NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外設
							 
}*/
//定時器3中斷服務程序
/*void TIM3_IRQHandler(void)   //TIM3中斷
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //檢查指定的TIM中斷發生與否:TIM 中斷源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中斷待處理位:TIM 中斷源 
		LED1=!LED1;
		}
}*/




//TIM3 PWM部分初始化 
//PWM輸出初始化
//arr：自動重裝值
//psc：時鐘預分頻數
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定時器3時鐘
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外設和AFIO復用功能模塊時鐘
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //設置該引腳為復用輸出功能,輸出TIM3 CH2的PWM脈衝波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //復用推輓輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //復用推輓輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //復用推輓輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //復用推輓輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //設置在下一個更新事件裝入活動的自動重裝載寄存器週期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //設置用來作為TIMx時鐘頻率除數的預分頻值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //設置時鐘分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上計數模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根據TIM_TimeBaseInitStruct中指定的參數初始化TIMx的時間基數單位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //選擇定時器模式:TIM脈衝寬度調制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比較輸出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //輸出極性:TIM輸出比較極性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根據T指定的參數初始化外設TIM3 OC2
	//初始化TIM3 Channel2 PWM模式	 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //選擇定時器模式:TIM脈衝寬度調制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比較輸出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //輸出極性:TIM輸出比較極性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根據T指定的參數初始化外設TIM3 OC2
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //選擇定時器模式:TIM脈衝寬度調制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比較輸出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //輸出極性:TIM輸出比較極性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根據T指定的參數初始化外設TIM3 OC2

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的預裝載寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的預裝載寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //選擇定時器模式:TIM脈衝寬度調制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比較輸出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //輸出極性:TIM輸出比較極性高
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根據T指定的參數初始化外設TIM3 OC2

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的預裝載寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的預裝載寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
	

}
