#include "IOI2C.h"
#include "delay.h"
  
/**************************實現函數********************************************
*函數原型:		void IIC_Init(void)
*功　　能:		初始化I2C對應的接口引腳。
*******************************************************************************/

void Delay(u32 count)//用於產生400KHzIIC信號所需要的延時
{
	while (count--);
}
void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			     
 	//配置PB6 PB7 為開漏輸出  刷新頻率為10Mhz
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//應用配置到GPIOB 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SDA_OUT();     //sda線輸出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
}

/**************************實現函數********************************************
*函數原型:		void IIC_Start(void)
*功　　能:		產生IIC起始信號
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda線輸出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	
	Delay(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	
	Delay(5);
	IIC_SCL=0;//鉗住I2C總線，準備發送或接收數據 
}

/**************************實現函數********************************************
*函數原型:		void IIC_Stop(void)
*功　　能:	    //產生IIC停止信號
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_OUT();//sda線輸出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	
		Delay(5);
	IIC_SCL=1; 
	IIC_SDA=1;//發送I2C總線結束信號
	
		Delay(5);							   	
}

/**************************實現函數********************************************
*函數原型:		u8 IIC_Wait_Ack(void)
*功　　能:	    等待應答信號到來 
//返回值：1，接收應答失敗
//        0，接收應答成功
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	SDA_IN();      //SDA設置為輸入  
	IIC_SDA=1;
		Delay(5);	  
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
		Delay(5);
	}  
	IIC_SCL=1;
	Delay(5); 
	IIC_SCL=0;//時鐘輸出0  
	return 0;  
} 

/**************************實現函數********************************************
*函數原型:		void IIC_Ack(void)
*功　　能:	    產生ACK應答
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
		Delay(5);
	IIC_SCL=1;
		Delay(5);
	IIC_SCL=0;
}
	
/**************************實現函數********************************************
*函數原型:		void IIC_NAck(void)
*功　　能:	    產生NACK應答
*******************************************************************************/	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	
		Delay(5);
	IIC_SCL=1;
		Delay(5);
	IIC_SCL=0;
}					 				     

/**************************實現函數********************************************
*函數原型:		void IIC_Send_Byte(u8 txd)
*功　　能:	    IIC發送一個字節
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t; 
		SDA_OUT(); 	    
    IIC_SCL=0;//拉低時鐘開始數據傳輸
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
			
		Delay(2);   
		IIC_SCL=1;
		Delay(5);
		IIC_SCL=0;	
		Delay(3);
    }	 
} 	 
   
/**************************實現函數********************************************
*函數原型:		u8 IIC_Read_Byte(unsigned char ack)
*功　　能:	    //讀1個字節，ack=1時，發送ACK，ack=0，發送nACK 
*******************************************************************************/  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA設置為輸入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        
		Delay(5);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		
		Delay(5); 
    }					 
    if (ack)
        IIC_Ack(); //發送ACK 
    else
        IIC_NAck();//發送nACK  
    return receive;
}

/**************************實現函數********************************************
*函數原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    讀取指定設備 指定寄存器的 length個值
輸入	dev  目標設備地址
		reg	  寄存器地址
		length 要讀的字節數
		*data  讀出的數據將要存放的指針
返回   讀出來的字節數量
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev<<1);	   //發送寫命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //發送地址
  IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte((dev<<1)+1);  //進入接收模式	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //帶ACK的讀數據
		 	else  data[count]=IIC_Read_Byte(0);	 //最後一個字節NACK
	}
    IIC_Stop();//產生一個停止條件
    return count;
}

/**************************實現函數********************************************
*函數原型:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    將多個字節寫入指定設備 指定寄存器
輸入	dev  目標設備地址
		reg	  寄存器地址
		length 要寫的字節數
		*data  將要寫的數據的首地址
返回   返回是否成功
*******************************************************************************/ 
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev<<1);	   //發送寫命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //發送地址
	IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
 }
	IIC_Stop();//產生一個停止條件

    return 1; //status == 0;
	
}
