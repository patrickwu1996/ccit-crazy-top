#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//�p�G�ݭn�ϥ�OS,�h�]�A�U�����Y���Y�i.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos �ϥ�	  
#endif
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

static u8  fac_us=0;							//us���ɭ�����			   
static u16 fac_ms=0;							//ms���ɭ�����,�bucos�U,�N��C�Ӹ`�窺ms��
	
	
#if SYSTEM_SUPPORT_OS							//�p�GSYSTEM_SUPPORT_OS�w�q�F,�����n���OS�F(������UCOS).
//��delay_us/delay_ms�ݭn���OS���ɭԻݭn�T�ӻPOS���������w�q�M��ƨӤ��
//�����O3�ӧ��w�q:
//    delay_osrunning:�Ω���OS��e�O�_���b�B��,�H�M�w�O�_�i�H�ϥά������
//delay_ostickspersec:�Ω���OS�]�w�������`��,delay_init�N�ھڳo�ӰѼƨӪ�l��systick
// delay_osintnesting:�Ω���OS���_�O�M�ŧO,�]�����_�̭����i�H�ի�,delay_ms�ϥθӰѼƨӨM�w�p��B��
//�M��O3�Ө��:
//  delay_osschedlock:�Ω���wOS���Ƚի�,�T��ի�
//delay_osschedunlock:�Ω����OS���Ƚի�,���s�}�ҽի�
//    delay_ostimedly:�Ω�OS����,�i�H�ް_���Ƚի�.

//���ҵ{�ȧ@UCOSII�MUCOSIII�����,��LOS,�Цۦ�Ѧҵ۲���
//���UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD�w�q�F,�����n���UCOSII				
#define delay_osrunning		OSRunning			//OS�O�_�B��аO,0,���B��;1,�b�B��
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS�����`��,�Y�C��իצ���
#define delay_osintnesting 	OSIntNesting		//���_�O�M�ŧO,�Y���_�O�M����
#endif

//���UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD�w�q�F,�����n���UCOSIII	
#define delay_osrunning		OSRunning			//OS�O�_�B��аO,0,���B��;1,�b�B��
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS�����`��,�Y�C��իצ���
#define delay_osintnesting 	OSIntNestingCtr		//���_�O�M�ŧO,�Y���_�O�M����
#endif


//us�ũ��ɮ�,�������Ƚի�(����_us�ũ���)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//�ϥ�UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII���覡,�T��իסA����_us����
#else											//�_�hUCOSII
	OSSchedLock();								//UCOSII���覡,�T��իסA����_us����
#endif
}

//us�ũ��ɮ�,��_���Ƚի�
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//�ϥ�UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII���覡,��_�ի�
#else											//�_�hUCOSII
	OSSchedUnlock();							//UCOSII���覡,��_�ի�
#endif
}

//�ե�OS�۱a�����ɨ�Ʃ���
//ticks:���ɪ��`���
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII���ɱĥζg���Ҧ�
#else
	OSTimeDly(ticks);							//UCOSII����
#endif 
}
 
//systick���_�A�Ȩ��,�ϥ�ucos�ɥΨ�
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS�}�l�]�F,�~���楿�`���ի׳B�z
	{
		OSIntEnter();							//�i�J���_
		OSTimeTick();       					//�ե�ucos�������A�ȵ{��               
		OSIntExit();       	 					//Ĳ�o���Ȥ����n���_
	}
}
#endif

			   
//��l�Ʃ�����
//��ϥ�OS���ɭ�,����Ʒ|��l��OS�������`��
//SYSTICK�������T�w��HCLK������1/8
//SYSCLK:�t�ή���
void delay_init1()
{
#if SYSTEM_SUPPORT_OS  							//�p�G�ݭn���OS.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//��ܥ~������  HCLK/8
	fac_us=SystemCoreClock/8000000;				//���t�ή�����1/8  
#if SYSTEM_SUPPORT_OS  							//�p�G�ݭn���OS.
	reload=SystemCoreClock/8000000;				//�C�������p�Ʀ��� ��쬰K	   
	reload*=1000000/delay_ostickspersec;		//�ھ�delay_ostickspersec�]�w���X�ɶ�
												//reload��24��H�s��,�̤j��:16777216,�b72M�U,���X1.86s���k	
	fac_ms=1000/delay_ostickspersec;			//�N��OS�i�H���ɪ��ֳ̤��	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//�}��SYSTICK���_
	SysTick->LOAD=reload; 						//�C1/delay_ostickspersec���_�@��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//�}��SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//�DOS�U,�N��C��ms�ݭn��systick������   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//�p�G�ݭn���OS.
//����nus
//nus���n���ɪ�us��.		    								   
void delay_us1(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD����	    	 
	ticks=nus*fac_us; 							//�ݭn���`���	  		 
	tcnt=0;
	delay_osschedlock();						//����OS�իסA����_us����
	told=SysTick->VAL;        					//��i�J�ɪ��p�ƾ���
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//�o�̪`�N�@�USYSTICK�O�@�ӻ���p�ƾ��N�i�H�F.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//�ɶ��W�L/����n���𪺮ɶ�,�h�h�X.
		}  
	};
	delay_osschedunlock();						//��_OS�ի�									    
}
//����nms
//nms:�n���ɪ�ms��
void delay_ms1(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//�p�GOS�w�g�b�]�F,�åB���O�b���_�̭�(���_�̭�������Ƚի�)	    
	{		 
		if(nms>=fac_ms)							//���ɪ��ɶ��j��OS���̤֮ɶ��g�� 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS����
		}
		nms%=fac_ms;							//OS�w�g�L�k���ѳo��p�����ɤF,�ĥδ��q�覡����    
	}
	delay_us((u32)(nms*1000));					//���q�覡����  
}
#else //����OS��
//����nus
//nus���n���ɪ�us��.		    								   
void delay_us1(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//�ɶ��[��	  		 
	SysTick->VAL=0x00;        					//�M�ŭp�ƾ�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//�}�l�˼�	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//���ݮɶ���F   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�����p�ƾ�
	SysTick->VAL =0X00;      					 //�M�ŭp�ƾ�	 
}
//����nms
//�`�Nnms���d��
//SysTick->LOAD��24��H�s��,�ҥH,�̤j���ɬ�:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��쬰Hz,nms��쬰ms
//��72M����U,nms<=1864 
void delay_ms1(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//�ɶ��[��(SysTick->LOAD��24bit)
	SysTick->VAL =0x00;							//�M�ŭp�ƾ�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//�}�l�˼�  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));		//���ݮɶ���F   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�����p�ƾ�
	SysTick->VAL =0X00;       					//�M�ŭp�ƾ�	  	    
} 
#endif 








































