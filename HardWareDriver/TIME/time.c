#include "time.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//定時器 驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//通用定時器3中斷初始化
//這裡時鐘選擇為APB1的2倍，而APB1為36M
//arr：自動重裝值。
//psc：時鐘預分頻數
//這裡使用的是定時器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //時鐘使能
	
	//定時器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //設置在下一個更新事件裝入活動的自動重裝載寄存器週期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //設置用來作為TIMx時鐘頻率除數的預分頻值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //設置時鐘分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上計數模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根據指定的參數初始化TIMx的時間基數單位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中斷,允許更新中斷

	//中斷優先級NVIC設置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中斷
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占優先級0級
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //從優先級3級
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
