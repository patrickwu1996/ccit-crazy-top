#include "time.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//�w�ɾ� �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/3
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//�q�Ωw�ɾ�3���_��l��
//�o�̮�����ܬ�APB1��2���A��APB1��36M
//arr�G�۰ʭ��˭ȡC
//psc�G�����w���W��
//�o�̨ϥΪ��O�w�ɾ�3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //�����ϯ�
	
	//�w�ɾ�TIM3��l��
	TIM_TimeBaseStructure.TIM_Period = arr; //�]�m�b�U�@�ӧ�s�ƥ�ˤJ���ʪ��۰ʭ��˸��H�s���g������	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //�]�m�Ψӧ@��TIMx�����W�v���ƪ��w���W��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //�]�m��������:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM�V�W�p�ƼҦ�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�ھګ��w���Ѽƪ�l��TIMx���ɶ���Ƴ��
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //�ϯ���w��TIM3���_,���\��s���_

	//���_�u����NVIC�]�m
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3���_
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //���e�u����0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�q�u����3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ�q�D�Q�ϯ�
	NVIC_Init(&NVIC_InitStructure);  //��l��NVIC�H�s��


	TIM_Cmd(TIM3, ENABLE);  //�ϯ�TIMx					 
}
