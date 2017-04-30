/* delay.c file
�s�g�̡GKevin
���}�Ghttp://RobotControl.taobao.com
�@��E-mail�Gzhaowenwin@139.com
�sĶ���ҡGMDK-Lite  Version: 4.50
�쪩�ɶ�: 2013-11-30
���աG 
�\��G
���Ѻ�T������API  ���L��� �M�@��ũ���
------------------------------------
 */
#include "delaysys.h"
	 
static u8  fac_us=0;//us���ɭ�����
static u16 fac_ms=0;//ms���ɭ�����

//��l�Ʃ�����
//SYSTICK�������T�w��HCLK������1/8
//SYSCLK:�t�ή���
/**************************��{���********************************************
*��ƭ쫬:		void SysTick_init(u8 SYSCLK,u16 nms)
*�\�@�@��:		��l�Ʃ���t�ΡA�ϩ��ɵ{�Ƕi�J�i�Ϊ��A
*******************************************************************************/
void SysTick_init(u8 SYSCLK,u16 nms)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	SysTick->VAL =0x00;           //�M�ŭp�ƾ�
	SysTick->LOAD = nms*SYSCLK*125;//72MHz,�̤j1864ms
	SysTick->CTRL=3;//bit2�M��,��ܥ~������  HCLK/8
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
//����nms
//�`�Nnms���d��
//SysTick->LOAD��24��H�s��,�ҥH,�̤j���ɬ�:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��쬰Hz,nms��쬰ms
/**************************��{���********************************************
*��ƭ쫬:		void delay_ms(u16 nms)
*�\�@�@��:		�@��ũ���  ����nms  nms<=1864 
*******************************************************************************/

void delay_ms(u16 nms)
{	 		  	  
  u32 Start = 	SysTick->VAL;
	u32 Span =(u32)nms*fac_ms;//�ɶ��[��(SysTick->LOAD��24bit)
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

//����nus
//nus���n���ɪ�us��.
/**************************��{���********************************************
*��ƭ쫬:		void delay_us(u32 nus)
*�\�@�@��:		�L��ũ���  ����nus  nms<=1864 
*******************************************************************************/		    								   
void delay_us(u32 nus)
{		
	u32 Start = 	SysTick->VAL;
	u32 Span =(u32)nus*fac_us;//�ɶ��[��(SysTick->LOAD��24bit)
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
