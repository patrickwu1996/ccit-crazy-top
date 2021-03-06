#include "24l01.h"
#include "delay.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//NRF24L01驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
    
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //發送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

//初始化24L01的IO口
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,G端口時鐘
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12上拉 防止W25X的干擾
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推輓輸出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化指定IO
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//上拉				
 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PG8 7 推輓 	  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化指定IO
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PG6 輸入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);//PG6,7,8上拉					 
		 
  SPI2_Init();    		//初始化SPI	 
 
	SPI_Cmd(SPI2, DISABLE); // SPI外設不使能

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI設置為雙線雙向全雙工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主機
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//發送接收8位幀結構
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//時鐘懸空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//數據捕獲於第1個時鐘沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信號由軟件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定義波特率預分頻的值:波特率預分頻值為16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//數據傳輸從MSB位開始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值計算的多項式
	SPI_Init(SPI2, &SPI_InitStructure);  //根據SPI_InitStruct中指定的參數初始化外設SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外設
			 
	NRF24L01_CE=0; 			//使能24L01
	NRF24L01_CSN=1;			//SPI片選取消  
	 		 	 
}
//檢測24L01是否存在
//返回值:0，成功;1，失敗	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //spi速度為9Mhz（24L01的最大SPI時鐘為10Mhz）   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//寫入5個字節的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //讀出寫入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//檢測24L01錯誤	
	return 0;		 //檢測到24L01
}	 	 
//SPI寫寄存器
//reg:指定寄存器地址
//value:寫入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //使能SPI傳輸
  	status =SPI2_ReadWriteByte(reg);//發送寄存器號 
  	SPI2_ReadWriteByte(value);      //寫入寄存器的值
  	NRF24L01_CSN=1;                 //禁止SPI傳輸	   
  	return(status);       			//返回狀態值
}
//讀取SPI寄存器值
//reg:要讀的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //使能SPI傳輸		
  	SPI2_ReadWriteByte(reg);   //發送寄存器號
  	reg_val=SPI2_ReadWriteByte(0XFF);//讀取寄存器內容
  	NRF24L01_CSN = 1;          //禁止SPI傳輸		    
  	return(reg_val);           //返回狀態值
}	
//在指定位置讀出指定長度的數據
//reg:寄存器(位置)
//*pBuf:數據指針
//len:數據長度
//返回值,此次讀到的狀態寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //使能SPI傳輸
  	status=SPI2_ReadWriteByte(reg);//發送寄存器值(位置),並讀取狀態值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//讀出數據
  	NRF24L01_CSN=1;       //關閉SPI傳輸
  	return status;        //返回讀到的狀態值
}
//在指定位置寫指定長度的數據
//reg:寄存器(位置)
//*pBuf:數據指針
//len:數據長度
//返回值,此次讀到的狀態寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //使能SPI傳輸
  	status = SPI2_ReadWriteByte(reg);//發送寄存器值(位置),並讀取狀態值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //寫入數據	 
  	NRF24L01_CSN = 1;       //關閉SPI傳輸
  	return status;          //返回讀到的狀態值
}				   
//啟動NRF24L01發送一次數據
//txbuf:待發送數據首地址
//返回值:發送完成狀況
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi速度為9Mhz（24L01的最大SPI時鐘為10Mhz）   
	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//寫數據到TX BUF  32個字節
 	NRF24L01_CE=1;//啟動發送	   
	while(NRF24L01_IRQ!=0);//等待發送完成
	sta=NRF24L01_Read_Reg(STATUS);  //讀取狀態寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中斷標誌
	if(sta&MAX_TX)//達到最大重發次數
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//發送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因發送失敗
}
//啟動NRF24L01發送一次數據
//txbuf:待發送數據首地址
//返回值:0，接收完成；其他，錯誤代碼
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度為9Mhz（24L01的最大SPI時鐘為10Mhz）   
	sta=NRF24L01_Read_Reg(STATUS);  //讀取狀態寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中斷標誌
	if(sta&RX_OK)//接收到數據
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//讀取數據
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//沒收到任何數據
}					    
//該函數初始化NRF24L01到RX模式
//設置RX地址,寫RX數據寬度,選擇RF頻道,波特率和LNA HCURR
//當CE變高後,即進入RX模式,並可以接收數據了		   
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE=0;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//寫RX節點地址
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自動應答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //設置RF通信頻率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//選擇通道0的有效數據寬度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//設置TX發射參數,0db增益,2Mbps,低噪聲增益開啟   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的參數;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF24L01_CE = 1; //CE為高,進入接收模式 
}						 
//該函數初始化NRF24L01到TX模式
//設置TX地址,寫TX數據寬度,設置RX自動應答的地址,填充TX發送數據,選擇RF頻道,波特率和LNA HCURR
//PWR_UP,CRC使能
//當CE變高後,即進入RX模式,並可以接收數據了		   
//CE為高大於10us,則啟動發送.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//寫TX節點地址 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //設置TX節點地址,主要為了使能ACK	  

  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自動應答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//設置自動重發間隔時間:500us + 86us;最大自動重發次數:10次
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //設置RF通道為40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //設置TX發射參數,0db增益,2Mbps,低噪聲增益開啟   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的參數;PWR_UP,EN_CRC,16BIT_CRC,接收模式,開啟所有中斷
	NRF24L01_CE=1;//CE為高,10us後啟動發送
}




