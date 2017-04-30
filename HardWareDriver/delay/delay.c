#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果需要使用OS,則包括下面的頭文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK STM32開發板
//使用SysTick的普通計數模式對延遲進行管理（適合STM32F10x系列）
//包括delay_us,delay_ms
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//創建日期:2010/1/1
//版本：V1.8
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改說明
//修正了中斷中調用出現死循環的錯誤
//防止延時不準確,採用do while結構!
//V1.3修改說明
//增加了對UCOSII延時的支持.
//如果使用ucosII,delay_init會自動設置SYSTICK的值,使之與ucos的TICKS_PER_SEC對應.
//delay_ms和delay_us也進行了針對ucos的改造.
//delay_us可以在ucos下使用,而且準確度很高,更重要的是沒有占用額外的定時器.
//delay_ms在ucos下,可以當成OSTimeDly來用,在未啟動ucos時,它採用delay_us實現,從而準確延時
//可以用來初始化外設,在啟動了ucos之後delay_ms根據延時的長短,選擇OSTimeDly實現或者delay_us實現.
//V1.4修改說明 20110929
//修改了使用ucos,但是ucos未啟動的時候,delay_ms中中斷無法響應的bug.
//V1.5修改說明 20120902
//在delay_us加入ucos上鎖，防止由於ucos打斷delay_us的執行，可能導致的延時不準。
//V1.6修改說明 20150109
//在delay_ms加入OSLockNesting判斷。
//V1.7修改說明 20150319
//修改OS支持方式,以支持任意OS(不限於UCOSII和UCOSIII,理論上任意OS都可以支持)
//添加:delay_osrunning/delay_ostickspersec/delay_osintnesting三個宏定義
//添加:delay_osschedlock/delay_osschedunlock/delay_ostimedly三個函數
//V1.8修改說明 20150519
//修正UCOSIII支持時的2個bug：
//delay_tickspersec改為：delay_ostickspersec
//delay_intnesting改為：delay_osintnesting
//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//us延時倍乘數			   
static u16 fac_ms=0;							//ms延時倍乘數,在ucos下,代表每個節拍的ms數
	
	
#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定義了,說明要支持OS了(不限於UCOS).
//當delay_us/delay_ms需要支持OS的時候需要三個與OS相關的宏定義和函數來支持
//首先是3個宏定義:
//    delay_osrunning:用於表示OS當前是否正在運行,以決定是否可以使用相關函數
//delay_ostickspersec:用於表示OS設定的時鐘節拍,delay_init將根據這個參數來初始哈systick
// delay_osintnesting:用於表示OS中斷嵌套級別,因為中斷裡面不可以調度,delay_ms使用該參數來決定如何運行
//然後是3個函數:
//  delay_osschedlock:用於鎖定OS任務調度,禁止調度
//delay_osschedunlock:用於解鎖OS任務調度,重新開啟調度
//    delay_ostimedly:用於OS延時,可以引起任務調度.

//本例程僅作UCOSII和UCOSIII的支持,其他OS,請自行參考著移植
//支持UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定義了,說明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否運行標記,0,不運行;1,在運行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS時鐘節拍,即每秒調度次數
#define delay_osintnesting 	OSIntNesting		//中斷嵌套級別,即中斷嵌套次數
#endif

//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定義了,說明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否運行標記,0,不運行;1,在運行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS時鐘節拍,即每秒調度次數
#define delay_osintnesting 	OSIntNestingCtr		//中斷嵌套級別,即中斷嵌套次數
#endif


//us級延時時,關閉任務調度(防止打斷us級延遲)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII的方式,禁止調度，防止打斷us延時
#else											//否則UCOSII
	OSSchedLock();								//UCOSII的方式,禁止調度，防止打斷us延時
#endif
}

//us級延時時,恢復任務調度
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII的方式,恢復調度
#else											//否則UCOSII
	OSSchedUnlock();							//UCOSII的方式,恢復調度
#endif
}

//調用OS自帶的延時函數延時
//ticks:延時的節拍數
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII延時採用週期模式
#else
	OSTimeDly(ticks);							//UCOSII延時
#endif 
}
 
//systick中斷服務函數,使用ucos時用到
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS開始跑了,才執行正常的調度處理
	{
		OSIntEnter();							//進入中斷
		OSTimeTick();       					//調用ucos的時鐘服務程序               
		OSIntExit();       	 					//觸發任務切換軟中斷
	}
}
#endif

			   
//初始化延遲函數
//當使用OS的時候,此函數會初始化OS的時鐘節拍
//SYSTICK的時鐘固定為HCLK時鐘的1/8
//SYSCLK:系統時鐘
void delay_init1()
{
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//選擇外部時鐘  HCLK/8
	fac_us=SystemCoreClock/8000000;				//為系統時鐘的1/8  
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	reload=SystemCoreClock/8000000;				//每秒鐘的計數次數 單位為K	   
	reload*=1000000/delay_ostickspersec;		//根據delay_ostickspersec設定溢出時間
												//reload為24位寄存器,最大值:16777216,在72M下,約合1.86s左右	
	fac_ms=1000/delay_ostickspersec;			//代表OS可以延時的最少單位	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//開啟SYSTICK中斷
	SysTick->LOAD=reload; 						//每1/delay_ostickspersec秒中斷一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//開啟SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//非OS下,代表每個ms需要的systick時鐘數   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
//延時nus
//nus為要延時的us數.		    								   
void delay_us1(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的節拍數	  		 
	tcnt=0;
	delay_osschedlock();						//阻止OS調度，防止打斷us延時
	told=SysTick->VAL;        					//剛進入時的計數器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//這裡注意一下SYSTICK是一個遞減的計數器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//時間超過/等於要延遲的時間,則退出.
		}  
	};
	delay_osschedunlock();						//恢復OS調度									    
}
//延時nms
//nms:要延時的ms數
void delay_ms1(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//如果OS已經在跑了,並且不是在中斷裡面(中斷裡面不能任務調度)	    
	{		 
		if(nms>=fac_ms)							//延時的時間大於OS的最少時間週期 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS延時
		}
		nms%=fac_ms;							//OS已經無法提供這麼小的延時了,採用普通方式延時    
	}
	delay_us((u32)(nms*1000));					//普通方式延時  
}
#else //不用OS時
//延時nus
//nus為要延時的us數.		    								   
void delay_us1(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//時間加載	  		 
	SysTick->VAL=0x00;        					//清空計數器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//開始倒數	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待時間到達   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//關閉計數器
	SysTick->VAL =0X00;      					 //清空計數器	 
}
//延時nms
//注意nms的範圍
//SysTick->LOAD為24位寄存器,所以,最大延時為:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK單位為Hz,nms單位為ms
//對72M條件下,nms<=1864 
void delay_ms1(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//時間加載(SysTick->LOAD為24bit)
	SysTick->VAL =0x00;							//清空計數器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//開始倒數  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));		//等待時間到達   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//關閉計數器
	SysTick->VAL =0X00;       					//清空計數器	  	    
} 
#endif 








































