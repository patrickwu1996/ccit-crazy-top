#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK Mini STM32�}�o�O
//�t�Τ��_���ճ]�m��		   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/10
//�����GV1.4
//���v�Ҧ��A�s�����s�C
//Copyright(C) ���I��l 2009-2019
//All rights reserved
//********************************************************************************  
//THUMB���O������׽s���p
//�ĥΦp�U��k��{����׽s���OWFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�����Ҧ����_
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//�}�ҩҦ����_
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//�]�m�̳��a�}
//addr:�̳��a�}
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
