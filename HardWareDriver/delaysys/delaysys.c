/* delay.c file
編寫者：Kevin
網址：http://RobotControl.taobao.com
作者E-mail：zhaowenwin@139.com
編譯環境：MDK-Lite  Version: 4.50
初版時間: 2013-11-30
測試： 
功能：
提供精確的延時API  有微秒級 和毫秒級延時
------------------------------------
 */
#include "delaysys.h"
	 
static u8  fac_us=0;//us延時倍乘數
static u16 fac_ms=0;//ms延時倍乘數

//初始化延遲函數
//SYSTICK的時鐘固定為HCLK時鐘的1/8
//SYSCLK:系統時鐘
/**************************實現函數********************************************
*函數原型:		void SysTick_init(u8 SYSCLK,u16 nms)
*功　　能:		初始化延遲系統，使延時程序進入可用狀態
*******************************************************************************/
void SysTick_init(u8 SYSCLK,u16 nms)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	SysTick->VAL =0x00;           //清空計數器
	SysTick->LOAD = nms*SYSCLK*125;//72MHz,最大1864ms
	SysTick->CTRL=3;//bit2清空,選擇外部時鐘  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
unsigned char ucTimeFlag=0,ucDelayFlag=0;
void SysTick_Handler(void) 
{      
	ucTimeFlag=1;	if (ucDelayFlag) ucDelayFlag--;
}			
unsigned char CheckSystemTick(void)
{	
	if (ucTimeFlag) 
	{
		ucTimeFlag = 0;
		return(1);
	}
	else 
		return(0);
}				    
//延時nms
//注意nms的範圍
//SysTick->LOAD為24位寄存器,所以,最大延時為:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK單位為Hz,nms單位為ms
/**************************實現函數********************************************
*函數原型:		void delay_ms(u16 nms)
*功　　能:		毫秒級延時  延時nms  nms<=1864 
*******************************************************************************/

void delay_ms(u16 nms)
{	 		  	  
  u32 Start = 	SysTick->VAL;
	u32 Span =(u32)nms*fac_ms;//時間加載(SysTick->LOAD為24bit)
	u32 End = 0;
		ucDelayFlag=Span/SysTick->LOAD;
		End = Span%SysTick->LOAD;
	if (Start>End)
	{
			End = Start-End;
		  while(ucDelayFlag);
			if (End<10)
				while(SysTick->VAL>10);
			else			
				while(SysTick->VAL>End); 
	}
	else
	{
			ucDelayFlag++;
			End = (Start+SysTick->LOAD)-End;
		  while(ucDelayFlag);			
			if (End<10)
				while(SysTick->VAL>10);
			else			
				while(SysTick->VAL>End); 
	}		  	    
}   

//延時nus
//nus為要延時的us數.
/**************************實現函數********************************************
*函數原型:		void delay_us(u32 nus)
*功　　能:		微秒級延時  延時nus  nms<=1864 
*******************************************************************************/		    								   
void delay_us(u32 nus)
{		
	u32 Start = 	SysTick->VAL;
	u32 Span =(u32)nus*fac_us;//時間加載(SysTick->LOAD為24bit)
	u32 End = 0;
		ucDelayFlag=Span/SysTick->LOAD;
		End = Span%SysTick->LOAD;
	if (Start>End)
	{
			End = Start-End;
		  while(ucDelayFlag);
			if (End<10)
				while(SysTick->VAL>10);
			else			
				while(SysTick->VAL>End); 
	}
	else
	{
			ucDelayFlag++;
			End = (Start+SysTick->LOAD)-End;
		  while(ucDelayFlag);			
			if (End<10)
				while(SysTick->VAL>10);
			else			
				while(SysTick->VAL>End); 
	}		  	    
}

//------------------End of File----------------------------
