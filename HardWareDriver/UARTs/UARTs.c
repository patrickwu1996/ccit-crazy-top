#include "UARTs.h"
#include "IOI2C.h"

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0; 

void NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure; 
          /* Enable the USART1 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

/**************************實現函數********************************************
*函數原型:		void Initial_UART1(u32 baudrate)
*功　　能:		初始化STM32-SDK開發板上的RS232接口
輸入參數：
		u32 baudrate   設置RS232串口的波特率
輸出參數：沒有	
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 UART1 模塊的時鐘  使能 UART1對應的引腳端口PA的時鐘*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* 配置UART1 的發送引腳
	 配置PA9 為復用輸出  刷新頻率50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  配置UART1 的接收引腳
	  配置PA10為浮地輸入 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART1的配置:
	  1.波特率為調用程序指定的輸入 baudrate;
	  2. 8位數據			  USART_WordLength_8b;
	  3.一個停止位			  USART_StopBits_1;
	  4. 無奇偶效驗			  USART_Parity_No ;
	  5.不使用硬件流控制	  USART_HardwareFlowControl_None;
	  6.使能發送和接收功能	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//應用配置到UART1
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        
    USART_ClearFlag(USART1,USART_FLAG_TC);
	//啟動UART1
  	USART_Cmd(USART1, ENABLE);
	NVIC_Configuration();
}

/**************************實現函數********************************************
*函數原型:		void UART1_Put_Char(unsigned char DataToSend)
*功　　能:		RS232發送一個字節
輸入參數：
		unsigned char DataToSend   要發送的字節數據
輸出參數：沒有	
*******************************************************************************/
void UART1_Put_Char(char DataToSend)
{
	//將要發送的字節寫到UART1的發送緩衝區
	USART_SendData(USART1, (unsigned char) DataToSend);
	//等待發送完成
  	while (!(USART1->SR & USART_FLAG_TXE));

	TxBuffer[count++] = DataToSend;  
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}


void UART1_Put_String(char *Str)
{
	//判斷Str指向的數據是否有效.
	while(*Str){
	//是否是迴車字符 如果是,則發送相應的迴車 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	Str++;
	}
}

//------------------------------------------------------

u8 chrTemp[250];

void CopeSerialData(unsigned char ucData);
void USART1_IRQHandler(void)
{
  
if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART1, TxBuffer[TxCounter++]); 
    if(TxCounter == count) 
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// 全部發送完成
		}
    USART_ClearITPendingBit(USART1, USART_IT_TXE); 
  }
	else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		CopeSerialData((unsigned char)USART1->DR);//處理數據
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
	USART_ClearITPendingBit(USART1,USART_IT_ORE);

}




//uart reicer flag
#define b_uart_head  0x80
#define b_rx_over    0x40

// USART Receiver buffer
#define RX_BUFFER_SIZE 100

u8 U2TxBuffer[258];
u8 U2TxCounter=0;
u8 U2count=0; 

void U2NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
		/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************實現函數********************************************
*函數原型:		void Initial_UART2(u32 baudrate)
*功　　能:		初始化STM32-SDK開發板上的RS232接口
輸入參數：
		u32 baudrate   設置RS232串口的波特率
輸出參數：沒有	
*******************************************************************************/
void Initial_UART2(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 UART2 模塊的時鐘  使能 UART2對應的引腳端口PA的時鐘*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* 配置UART2 的發送引腳
	 配置PA9 為復用輸出  刷新頻率50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  配置UART2 的接收引腳
	  配置PA10為浮地輸入 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART2的配置:
	  1.波特率為調用程序指定的輸入 baudrate;
	  2. 8位數據			  USART_WordLength_8b;
	  3.一個停止位			  USART_StopBits_1;
	  4. 無奇偶效驗			  USART_Parity_No ;
	  5.不使用硬件流控制	  USART_HardwareFlowControl_None;
	  6.使能發送和接收功能	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//應用配置到UART2
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);        
    USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//使能接收中斷
	//啟動UART2
  	USART_Cmd(USART2, ENABLE);
	U2NVIC_Configuration();
}

volatile unsigned char RC_Flag;
//------------------------------------------------------
void USART2_IRQHandler(void)
{
  unsigned char data;
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART2, U2TxBuffer[U2TxCounter++]);                    

    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_TXE); 

    if(U2TxCounter == U2count)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }    
  }else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		data=USART_ReceiveData(USART2);
 
  /* Clear the USART1 transmit interrupt */
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}
//------------------End of File----------------------------
