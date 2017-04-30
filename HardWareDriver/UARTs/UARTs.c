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

/**************************��{���********************************************
*��ƭ쫬:		void Initial_UART1(u32 baudrate)
*�\�@�@��:		��l��STM32-SDK�}�o�O�W��RS232���f
��J�ѼơG
		u32 baudrate   �]�mRS232��f���i�S�v
��X�ѼơG�S��	
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* �ϯ� UART1 �Ҷ�������  �ϯ� UART1�������޸}�ݤfPA������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* �t�mUART1 ���o�e�޸}
	 �t�mPA9 ���_�ο�X  ��s�W�v50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  �t�mUART1 �������޸}
	  �t�mPA10���B�a��J 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART1���t�m:
	  1.�i�S�v���եε{�ǫ��w����J baudrate;
	  2. 8��ƾ�			  USART_WordLength_8b;
	  3.�@�Ӱ����			  USART_StopBits_1;
	  4. �L�_������			  USART_Parity_No ;
	  5.���ϥεw��y����	  USART_HardwareFlowControl_None;
	  6.�ϯ�o�e�M�����\��	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//���ΰt�m��UART1
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        
    USART_ClearFlag(USART1,USART_FLAG_TC);
	//�Ұ�UART1
  	USART_Cmd(USART1, ENABLE);
	NVIC_Configuration();
}

/**************************��{���********************************************
*��ƭ쫬:		void UART1_Put_Char(unsigned char DataToSend)
*�\�@�@��:		RS232�o�e�@�Ӧr�`
��J�ѼơG
		unsigned char DataToSend   �n�o�e���r�`�ƾ�
��X�ѼơG�S��	
*******************************************************************************/
void UART1_Put_Char(char DataToSend)
{
	//�N�n�o�e���r�`�g��UART1���o�e�w�İ�
	USART_SendData(USART1, (unsigned char) DataToSend);
	//���ݵo�e����
  	while (!(USART1->SR & USART_FLAG_TXE));

	TxBuffer[count++] = DataToSend;  
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}


void UART1_Put_String(char *Str)
{
	//�P�_Str���V���ƾڬO�_����.
	while(*Str){
	//�O�_�O�j���r�� �p�G�O,�h�o�e�������j�� 0x0d 0x0a
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
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// �����o�e����
		}
    USART_ClearITPendingBit(USART1, USART_IT_TXE); 
  }
	else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		CopeSerialData((unsigned char)USART1->DR);//�B�z�ƾ�
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

/**************************��{���********************************************
*��ƭ쫬:		void Initial_UART2(u32 baudrate)
*�\�@�@��:		��l��STM32-SDK�}�o�O�W��RS232���f
��J�ѼơG
		u32 baudrate   �]�mRS232��f���i�S�v
��X�ѼơG�S��	
*******************************************************************************/
void Initial_UART2(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* �ϯ� UART2 �Ҷ�������  �ϯ� UART2�������޸}�ݤfPA������*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* �t�mUART2 ���o�e�޸}
	 �t�mPA9 ���_�ο�X  ��s�W�v50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  �t�mUART2 �������޸}
	  �t�mPA10���B�a��J 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART2���t�m:
	  1.�i�S�v���եε{�ǫ��w����J baudrate;
	  2. 8��ƾ�			  USART_WordLength_8b;
	  3.�@�Ӱ����			  USART_StopBits_1;
	  4. �L�_������			  USART_Parity_No ;
	  5.���ϥεw��y����	  USART_HardwareFlowControl_None;
	  6.�ϯ�o�e�M�����\��	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//���ΰt�m��UART2
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);        
    USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//�ϯ౵�����_
	//�Ұ�UART2
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
