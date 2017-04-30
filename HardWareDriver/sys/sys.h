#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK STM32開發板		   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/8/18
//版本：V1.7
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		0		//定義系統文件夾是否支持UCOS
																	    
	 
//位帶操作,實現51類似的GPIO控制功能
//具體實現思想,參考<<CM3權威指南>>第五章(87頁~92頁).
//IO口操作宏定義
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只對單一的IO口!
//確保n的值小於16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //輸出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //輸入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //輸出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //輸入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //輸出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //輸入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //輸出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //輸入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //輸出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //輸入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //輸出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //輸入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //輸出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //輸入

//以下為匯編函數
void WFI_SET(void);		//執行WFI指令
void INTX_DISABLE(void);//關閉所有中斷
void INTX_ENABLE(void);	//開啟所有中斷
void MSR_MSP(u32 addr);	//設置堆棧地址

#endif
