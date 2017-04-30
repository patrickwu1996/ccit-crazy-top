#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//SPI驅動 代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//以下是SPI模塊的初始化代碼，配置成主機模式，訪問SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//這裡針是對SPI2的初始化



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB時鐘使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2時鐘使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15復用推輓輸出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //設置SPI單向或者雙向的數據模式:SPI設置為雙線雙向全雙工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//設置SPI工作模式:設置為主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//設置SPI的數據大小:SPI發送接收8位幀結構
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步時鐘的空閒狀態為高電平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步時鐘的第二個跳變沿（上升或下降）數據被采樣
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信號由硬件（NSS管腳）還是軟件（使用SSI位）管理:內部NSS信號有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定義波特率預分頻的值:波特率預分頻值為256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定數據傳輸從MSB位還是LSB位開始:數據傳輸從MSB位開始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值計算的多項式
	SPI_Init(SPI2, &SPI_InitStructure);  //根據SPI_InitStruct中指定的參數初始化外設SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外設
	
	SPI2_ReadWriteByte(0xff);//啟動傳輸		 
 

}   
//SPI 速度設置函數
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分頻   
//SPI_BaudRatePrescaler_8   8分頻   
//SPI_BaudRatePrescaler_16  16分頻  
//SPI_BaudRatePrescaler_256 256分頻 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//設置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx 讀寫一個字節
//TxData:要寫入的字節
//返回值:讀取到的字節
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //檢查指定的SPI標誌位設置與否:發送緩存空標誌位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //通過外設SPIx發送一個數據
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//檢查指定的SPI標誌位設置與否:接受緩存非空標誌位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通過SPIx最近接收的數據					    
}































