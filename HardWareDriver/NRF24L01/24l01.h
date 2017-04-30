#ifndef __24L01_H
#define __24L01_H	 		  
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK��ĥSTM32�}�o�OV3
//NRF24L01�X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2015/1/17
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�H�s���ާ@�R�O
#define NRF_READ_REG    0x00  //Ū�t�m�H�s��,�C5�쬰�H�s���a�}
#define NRF_WRITE_REG   0x20  //�g�t�m�H�s��,�C5�쬰�H�s���a�}
#define RD_RX_PLOAD     0x61  //ŪRX���ļƾ�,1~32�r�`
#define WR_TX_PLOAD     0xA0  //�gTX���ļƾ�,1~32�r�`
#define FLUSH_TX        0xE1  //�M��TX FIFO�H�s��.�o�g�Ҧ��U��
#define FLUSH_RX        0xE2  //�M��RX FIFO�H�s��.�����Ҧ��U��
#define REUSE_TX_PL     0xE3  //���s�ϥΤW�@�]�ƾ�,CE����,�ƾڥ]�Q���_�o�e.
#define NOP             0xFF  //�žާ@,�i�H�Ψ�Ū���A�H�s��	 
//SPI(NRF24L01)�H�s���a�}
#define CONFIG          0x00  //�t�m�H�s���a�};bit0:1�����Ҧ�,0�o�g�Ҧ�;bit1:�q���;bit2:CRC�Ҧ�;bit3:CRC�ϯ�;
                              //bit4:���_MAX_RT(�F��̤j���o���Ƥ��_)�ϯ�;bit5:���_TX_DS�ϯ�;bit6:���_RX_DR�ϯ�
#define EN_AA           0x01  //�ϯ�۰������\��  bit0~5,�����q�D0~5
#define EN_RXADDR       0x02  //�����a�}���\,bit0~5,�����q�D0~5
#define SETUP_AW        0x03  //�]�m�a�}�e��(�Ҧ��ƾڳq�D):bit1,0:00,3�r�`;01,4�r�`;02,5�r�`;
#define SETUP_RETR      0x04  //�إߦ۰ʭ��o;bit3:0,�۰ʭ��o�p�ƾ�;bit7:4,�۰ʭ��o���� 250*x+86us
#define RF_CH           0x05  //RF�q�D,bit6:0,�u�@�q�D�W�v;
#define RF_SETUP        0x06  //RF�H�s��;bit3:�ǿ�t�v(0:1Mbps,1:2Mbps);bit2:1,�o�g�\�v;bit0:�C���n��j���W�q
#define STATUS          0x07  //���A�H�s��;bit0:TX FIFO���лx;bit3:1,�����ƾڳq�D��(�̤j:6);bit4,�F��̦h�����o
                              //bit5:�ƾڵo�e�������_;bit6:�����ƾڤ��_;
#define MAX_TX  		0x10  //�F��̤j�o�e���Ƥ��_
#define TX_OK   		0x20  //TX�o�e�������_
#define RX_OK   		0x40  //������ƾڤ��_

#define OBSERVE_TX      0x08  //�o�e�˴��H�s��,bit7:4,�ƾڥ]�ᥢ�p�ƾ�;bit3:0,���o�p�ƾ�
#define CD              0x09  //���i�˴��H�s��,bit0,���i�˴�;
#define RX_ADDR_P0      0x0A  //�ƾڳq�D0�����a�},�̤j����5�Ӧr�`,�C�r�`�b�e
#define RX_ADDR_P1      0x0B  //�ƾڳq�D1�����a�},�̤j����5�Ӧr�`,�C�r�`�b�e
#define RX_ADDR_P2      0x0C  //�ƾڳq�D2�����a�},�̧C�r�`�i�]�m,���r�`,�����PRX_ADDR_P1[39:8]�۵�;
#define RX_ADDR_P3      0x0D  //�ƾڳq�D3�����a�},�̧C�r�`�i�]�m,���r�`,�����PRX_ADDR_P1[39:8]�۵�;
#define RX_ADDR_P4      0x0E  //�ƾڳq�D4�����a�},�̧C�r�`�i�]�m,���r�`,�����PRX_ADDR_P1[39:8]�۵�;
#define RX_ADDR_P5      0x0F  //�ƾڳq�D5�����a�},�̧C�r�`�i�]�m,���r�`,�����PRX_ADDR_P1[39:8]�۵�;
#define TX_ADDR         0x10  //�o�e�a�}(�C�r�`�b�e),ShockBurstTM�Ҧ��U,RX_ADDR_P0�P���a�}�۵�
#define RX_PW_P0        0x11  //�����ƾڳq�D0���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define RX_PW_P1        0x12  //�����ƾڳq�D1���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define RX_PW_P2        0x13  //�����ƾڳq�D2���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define RX_PW_P3        0x14  //�����ƾڳq�D3���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define RX_PW_P4        0x15  //�����ƾڳq�D4���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define RX_PW_P5        0x16  //�����ƾڳq�D5���ļƾڼe��(1~32�r�`),�]�m��0�h�D�k
#define NRF_FIFO_STATUS 0x17  //FIFO���A�H�s��;bit0,RX FIFO�H�s���żлx;bit1,RX FIFO���лx;bit2,3,�O�d
                              //bit4,TX FIFO�żлx;bit5,TX FIFO���лx;bit6,1,�`���o�e�W�@�ƾڥ].0,���`��;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01�ާ@�u
#define NRF24L01_CE   PAout(0) //24L01����H��
#define NRF24L01_CSN  PAout(1) //SPI����H��	   
#define NRF24L01_IRQ  PAin(2)  //IRQ�D���ƾڿ�J
//24L01�o�e�����ƾڼe�שw�q
#define TX_ADR_WIDTH    5   	//5�r�`���a�}�e��
#define RX_ADR_WIDTH    5   	//5�r�`���a�}�e��
#define TX_PLOAD_WIDTH  32  	//32�r�`���Τ�ƾڼe��
#define RX_PLOAD_WIDTH  32  	//32�r�`���Τ�ƾڼe��
									   	   

void NRF24L01_Init(void);						//��l��
void NRF24L01_RX_Mode(void);					//�t�m�������Ҧ�
void NRF24L01_TX_Mode(void);					//�t�m���o�e�Ҧ�
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);//�g�ƾڰ�
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);	//Ū�ƾڰ�		  
u8 NRF24L01_Read_Reg(u8 reg);					//Ū�H�s��
u8 NRF24L01_Write_Reg(u8 reg, u8 value);		//�g�H�s��
u8 NRF24L01_Check(void);						//�ˬd24L01�O�_�s�b
u8 NRF24L01_TxPacket(u8 *txbuf);				//�o�e�@�ӥ]���ƾ�
u8 NRF24L01_RxPacket(u8 *rxbuf);				//�����@�ӥ]���ƾ�
#endif











