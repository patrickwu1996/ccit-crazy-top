#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//SPI�X�� �N�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/9
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//�H�U�OSPI�Ҷ�����l�ƥN�X�A�t�m���D���Ҧ��A�X��SD Card/W25Q64/NRF24L01						  
//SPI�f��l��
//�o�̰w�O��SPI2����l��



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB�����ϯ� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2�����ϯ� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15�_�α�����X 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15�W��

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //�]�mSPI��V�Ϊ����V���ƾڼҦ�:SPI�]�m�����u���V�����u
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//�]�mSPI�u�@�Ҧ�:�]�m���DSPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//�]�mSPI���ƾڤj�p:SPI�o�e����8��V���c
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//���P�B�������Ŷ����A�����q��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���P�B�������ĤG�Ӹ��ܪu�]�W�ɩΤU���^�ƾڳQ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�H���ѵw��]NSS�޸}�^�٬O�n��]�ϥ�SSI��^�޲z:����NSS�H����SSI�챱��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//�w�q�i�S�v�w���W����:�i�S�v�w���W�Ȭ�256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���w�ƾڶǿ�qMSB���٬OLSB��}�l:�ƾڶǿ�qMSB��}�l
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC�ȭp�⪺�h����
	SPI_Init(SPI2, &SPI_InitStructure);  //�ھ�SPI_InitStruct�����w���Ѽƪ�l�ƥ~�]SPIx�H�s��
 
	SPI_Cmd(SPI2, ENABLE); //�ϯ�SPI�~�]
	
	SPI2_ReadWriteByte(0xff);//�Ұʶǿ�		 
 

}   
//SPI �t�׳]�m���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2���W   
//SPI_BaudRatePrescaler_8   8���W   
//SPI_BaudRatePrescaler_16  16���W  
//SPI_BaudRatePrescaler_256 256���W 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//�]�mSPI2�t�� 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx Ū�g�@�Ӧr�`
//TxData:�n�g�J���r�`
//��^��:Ū���쪺�r�`
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //�ˬd���w��SPI�лx��]�m�P�_:�o�e�w�s�żлx��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //�q�L�~�]SPIx�o�e�@�Ӽƾ�
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//�ˬd���w��SPI�лx��]�m�P�_:�����w�s�D�żлx��
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //��^�q�LSPIx�̪񱵦����ƾ�					    
}































