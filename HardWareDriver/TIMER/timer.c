#include "timer.h"
#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//�w�ɾ� �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/4
//�����GV1.1
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,�W�[TIM3_PWM_Init��ơC
//2,�W�[LED0_PWM_VAL���w�q�A����TIM3_CH2�߼e									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//�q�Ωw�ɾ�3���_��l��
//�o�̮�����ܬ�APB1��2���A��APB1��36M
//arr�G�۰ʭ��˭ȡC
//psc�G�����w���W��
//�o�̨ϥΪ��O�w�ɾ�3!
/*void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //�����ϯ�

	TIM_TimeBaseStructure.TIM_Period = arr; //�]�m�b�U�@�ӧ�s�ƥ�ˤJ���ʪ��۰ʭ��˸��H�s���g������	 �p�ƨ�5000��500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //�]�m�Ψӧ@��TIMx�����W�v���ƪ��w���W��  10Khz���p���W�v  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //�]�m��������:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM�V�W�p�ƼҦ�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�ھ�TIM_TimeBaseInitStruct�����w���Ѽƪ�l��TIMx���ɶ���Ƴ��
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //�ϯ���w��TIM3���_,���\��s���_

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3���_
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���e�u����0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�q�u����3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ�q�D�Q�ϯ�
	NVIC_Init(&NVIC_InitStructure);  //�ھ�NVIC_InitStruct�����w���Ѽƪ�l�ƥ~�]NVIC�H�s��

	TIM_Cmd(TIM3, ENABLE);  //�ϯ�TIMx�~�]
							 
}*/
//�w�ɾ�3���_�A�ȵ{��
/*void TIM3_IRQHandler(void)   //TIM3���_
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //�ˬd���w��TIM���_�o�ͻP�_:TIM ���_�� 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //�M��TIMx�����_�ݳB�z��:TIM ���_�� 
		LED1=!LED1;
		}
}*/




//TIM3 PWM������l�� 
//PWM��X��l��
//arr�G�۰ʭ��˭�
//psc�G�����w���W��
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//�ϯ�w�ɾ�3����
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //�ϯ�GPIO�~�]�MAFIO�_�Υ\��Ҷ�����
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3�������M�g  TIM3_CH2->PB5    
 
   //�]�m�Ӥ޸}���_�ο�X�\��,��XTIM3 CH2��PWM�߽Īi��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�_�α�����X
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�_�α�����X
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�_�α�����X
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIO
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�_�α�����X
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIO
 
   //��l��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //�]�m�b�U�@�ӧ�s�ƥ�ˤJ���ʪ��۰ʭ��˸��H�s���g������
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //�]�m�Ψӧ@��TIMx�����W�v���ƪ��w���W�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //�]�m��������:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM�V�W�p�ƼҦ�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //�ھ�TIM_TimeBaseInitStruct�����w���Ѽƪ�l��TIMx���ɶ���Ƴ��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //��ܩw�ɾ��Ҧ�:TIM�߽ļe�׽ը�Ҧ�2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�����X�ϯ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��X����:TIM��X������ʰ�
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //�ھ�T���w���Ѽƪ�l�ƥ~�]TIM3 OC2
	//��l��TIM3 Channel2 PWM�Ҧ�	 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //��ܩw�ɾ��Ҧ�:TIM�߽ļe�׽ը�Ҧ�2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�����X�ϯ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��X����:TIM��X������ʰ�
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //�ھ�T���w���Ѽƪ�l�ƥ~�]TIM3 OC2
	//��l��TIM3 Channel2 PWM�Ҧ�	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //��ܩw�ɾ��Ҧ�:TIM�߽ļe�׽ը�Ҧ�2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�����X�ϯ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��X����:TIM��X������ʰ�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //�ھ�T���w���Ѽƪ�l�ƥ~�]TIM3 OC2

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //�ϯ�TIM3�bCCR2�W���w�˸��H�s��
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //�ϯ�TIM3�bCCR2�W���w�˸��H�s��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //��ܩw�ɾ��Ҧ�:TIM�߽ļe�׽ը�Ҧ�2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�����X�ϯ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��X����:TIM��X������ʰ�
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //�ھ�T���w���Ѽƪ�l�ƥ~�]TIM3 OC2

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //�ϯ�TIM3�bCCR2�W���w�˸��H�s��
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //�ϯ�TIM3�bCCR2�W���w�˸��H�s��
 
	TIM_Cmd(TIM4, ENABLE);  //�ϯ�TIM3
	

}
