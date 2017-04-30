#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK Mini STM32開發板
//系統中斷分組設置化		   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/10
//版本：V1.4
//版權所有，盜版必究。
//Copyright(C) 正點原子 2009-2019
//All rights reserved
//********************************************************************************  
//THUMB指令不支持匯編內聯
//採用如下方法實現執行匯編指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//關閉所有中斷
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//開啟所有中斷
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//設置棧頂地址
//addr:棧頂地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
