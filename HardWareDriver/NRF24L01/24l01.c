#include "24l01.h"
#include "delay.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�O
//NRF24L01�X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2012/9/13
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
    
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //�o�e�a�}
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

//��l��24L01��IO�f
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //�ϯ�PB,G�ݤf����
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12�W�� ����W25X���z�Z
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������X
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��l�ƫ��wIO
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//�W��				
 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PG8 7 ���� 	  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��l�ƫ��wIO
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PG6 ��J  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);//PG6,7,8�W��					 
		 
  SPI2_Init();    		//��l��SPI	 
 
	SPI_Cmd(SPI2, DISABLE); // SPI�~�]���ϯ�

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI�]�m�����u���V�����u
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI�D��
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//�o�e����8��V���c
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//�����a�ŧC
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//�ƾڮ�����1�Ӯ����u
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�H���ѳn�󱱨�
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//�w�q�i�S�v�w���W����:�i�S�v�w���W�Ȭ�16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//�ƾڶǿ�qMSB��}�l
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC�ȭp�⪺�h����
	SPI_Init(SPI2, &SPI_InitStructure);  //�ھ�SPI_InitStruct�����w���Ѽƪ�l�ƥ~�]SPIx�H�s��
 
	SPI_Cmd(SPI2, ENABLE); //�ϯ�SPI�~�]
			 
	NRF24L01_CE=0; 			//�ϯ�24L01
	NRF24L01_CSN=1;			//SPI�������  
	 		 	 
}
//�˴�24L01�O�_�s�b
//��^��:0�A���\;1�A����	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //spi�t�׬�9Mhz�]24L01���̤jSPI������10Mhz�^   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//�g�J5�Ӧr�`���a�}.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //Ū�X�g�J���a�}  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//�˴�24L01���~	
	return 0;		 //�˴���24L01
}	 	 
//SPI�g�H�s��
//reg:���w�H�s���a�}
//value:�g�J����
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //�ϯ�SPI�ǿ�
  	status =SPI2_ReadWriteByte(reg);//�o�e�H�s���� 
  	SPI2_ReadWriteByte(value);      //�g�J�H�s������
  	NRF24L01_CSN=1;                 //�T��SPI�ǿ�	   
  	return(status);       			//��^���A��
}
//Ū��SPI�H�s����
//reg:�nŪ���H�s��
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //�ϯ�SPI�ǿ�		
  	SPI2_ReadWriteByte(reg);   //�o�e�H�s����
  	reg_val=SPI2_ReadWriteByte(0XFF);//Ū���H�s�����e
  	NRF24L01_CSN = 1;          //�T��SPI�ǿ�		    
  	return(reg_val);           //��^���A��
}	
//�b���w��mŪ�X���w���ת��ƾ�
//reg:�H�s��(��m)
//*pBuf:�ƾګ��w
//len:�ƾڪ���
//��^��,����Ū�쪺���A�H�s���� 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //�ϯ�SPI�ǿ�
  	status=SPI2_ReadWriteByte(reg);//�o�e�H�s����(��m),��Ū�����A��   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//Ū�X�ƾ�
  	NRF24L01_CSN=1;       //����SPI�ǿ�
  	return status;        //��^Ū�쪺���A��
}
//�b���w��m�g���w���ת��ƾ�
//reg:�H�s��(��m)
//*pBuf:�ƾګ��w
//len:�ƾڪ���
//��^��,����Ū�쪺���A�H�s����
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //�ϯ�SPI�ǿ�
  	status = SPI2_ReadWriteByte(reg);//�o�e�H�s����(��m),��Ū�����A��
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //�g�J�ƾ�	 
  	NRF24L01_CSN = 1;       //����SPI�ǿ�
  	return status;          //��^Ū�쪺���A��
}				   
//�Ұ�NRF24L01�o�e�@���ƾ�
//txbuf:�ݵo�e�ƾڭ��a�}
//��^��:�o�e�������p
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi�t�׬�9Mhz�]24L01���̤jSPI������10Mhz�^   
	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//�g�ƾڨ�TX BUF  32�Ӧr�`
 	NRF24L01_CE=1;//�Ұʵo�e	   
	while(NRF24L01_IRQ!=0);//���ݵo�e����
	sta=NRF24L01_Read_Reg(STATUS);  //Ū�����A�H�s������	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //�M��TX_DS��MAX_RT���_�лx
	if(sta&MAX_TX)//�F��̤j���o����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//�M��TX FIFO�H�s�� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�o�e����
	{
		return TX_OK;
	}
	return 0xff;//��L��]�o�e����
}
//�Ұ�NRF24L01�o�e�@���ƾ�
//txbuf:�ݵo�e�ƾڭ��a�}
//��^��:0�A���������F��L�A���~�N�X
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�t�׬�9Mhz�]24L01���̤jSPI������10Mhz�^   
	sta=NRF24L01_Read_Reg(STATUS);  //Ū�����A�H�s������    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //�M��TX_DS��MAX_RT���_�лx
	if(sta&RX_OK)//������ƾ�
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//Ū���ƾ�
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//�M��RX FIFO�H�s�� 
		return 0; 
	}	   
	return 1;//�S�������ƾ�
}					    
//�Ө�ƪ�l��NRF24L01��RX�Ҧ�
//�]�mRX�a�},�gRX�ƾڼe��,���RF�W�D,�i�S�v�MLNA HCURR
//��CE�ܰ���,�Y�i�JRX�Ҧ�,�åi�H�����ƾڤF		   
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE=0;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//�gRX�`�I�a�}
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //�ϯ�q�D0���۰�����    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//�ϯ�q�D0�������a�}  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //�]�mRF�q�H�W�v		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//��ܳq�D0�����ļƾڼe�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//�]�mTX�o�g�Ѽ�,0db�W�q,2Mbps,�C���n�W�q�}��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//�t�m�򥻤u�@�Ҧ����Ѽ�;PWR_UP,EN_CRC,16BIT_CRC,�����Ҧ� 
  	NRF24L01_CE = 1; //CE����,�i�J�����Ҧ� 
}						 
//�Ө�ƪ�l��NRF24L01��TX�Ҧ�
//�]�mTX�a�},�gTX�ƾڼe��,�]�mRX�۰��������a�},��RTX�o�e�ƾ�,���RF�W�D,�i�S�v�MLNA HCURR
//PWR_UP,CRC�ϯ�
//��CE�ܰ���,�Y�i�JRX�Ҧ�,�åi�H�����ƾڤF		   
//CE�����j��10us,�h�Ұʵo�e.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//�gTX�`�I�a�} 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //�]�mTX�`�I�a�},�D�n���F�ϯ�ACK	  

  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //�ϯ�q�D0���۰�����    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //�ϯ�q�D0�������a�}  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�]�m�۰ʭ��o���j�ɶ�:500us + 86us;�̤j�۰ʭ��o����:10��
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //�]�mRF�q�D��40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //�]�mTX�o�g�Ѽ�,0db�W�q,2Mbps,�C���n�W�q�}��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //�t�m�򥻤u�@�Ҧ����Ѽ�;PWR_UP,EN_CRC,16BIT_CRC,�����Ҧ�,�}�ҩҦ����_
	NRF24L01_CE=1;//CE����,10us��Ұʵo�e
}




