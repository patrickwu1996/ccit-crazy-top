 #include "adc.h"
 #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//ADC �N�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/7
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
		   
//��l��ADC
//�o�̧ڭ̶ȥH�W�h�q�D����
//�ڭ��q�{�N�}�ҳq�D0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //�ϯ�ADC1�q�D����
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //�]�mADC���W�]�l6 72M/6=12,ADC�̤j�ɶ�����W�L14M

	//PA1 �@�������q�D��J�޸}                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//������J�޸}
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //�_��ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC�u�@�Ҧ�:ADC1�MADC2�u�@�b�W�߼Ҧ�
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//�Ҽ��ഫ�u�@�b��q�D�Ҧ�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//�Ҽ��ഫ�u�@�b�榸�ഫ�Ҧ�
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ഫ�ѳn��Ӥ��O�~��Ĳ�o�Ұ�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�ƾڥk���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//���Ƕi��W�h�ഫ��ADC�q�D���ƥ�
	ADC_Init(ADC1, &ADC_InitStructure);	//�ھ�ADC_InitStruct�����w���Ѽƪ�l�ƥ~�]ADCx���H�s��   

  
	ADC_Cmd(ADC1, ENABLE);	//�ϯ���w��ADC1
	
	ADC_ResetCalibration(ADC1);	//�ϯ�_��շ�  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//���ݴ_��շǵ���
	
	ADC_StartCalibration(ADC1);	 //�}��AD�շ�
 
	while(ADC_GetCalibrationStatus(ADC1));	 //���ݮշǵ���
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�ϯ���w��ADC1���n���ഫ�Ұʥ\��

}				  
//��oADC��
//ch:�q�D�� 0~3
u16 Get_Adc(u8 ch)   
{
  	//�]�m���wADC���W�h�ճq�D�A�@�ӧǦC�A���ˮɶ�
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_28Cycles5 );	//ADC1,ADC�q�D,���ˮɶ���239.5�g��	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�ϯ���w��ADC1���n���ഫ�Ұʥ\��	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�����ഫ����

	return ADC_GetConversionValue(ADC1);	//��^�̪�@��ADC1�W�h�ժ��ഫ���G
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
	//	delay_ms(5);
	}
	return temp_val/times;
} 	 



























