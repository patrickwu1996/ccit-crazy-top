#include "IOI2C.h"
#include "delay.h"
  
/**************************��{���********************************************
*��ƭ쫬:		void IIC_Init(void)
*�\�@�@��:		��l��I2C���������f�޸}�C
*******************************************************************************/

void Delay(u32 count)//�Ω󲣥�400KHzIIC�H���һݭn������
{
	while (count--);
}
void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			     
 	//�t�mPB6 PB7 ���}�|��X  ��s�W�v��10Mhz
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ΰt�m��GPIOB 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SDA_OUT();     //sda�u��X
	IIC_SDA=1;	  	  
	IIC_SCL=1;
}

/**************************��{���********************************************
*��ƭ쫬:		void IIC_Start(void)
*�\�@�@��:		����IIC�_�l�H��
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda�u��X
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	
	Delay(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	
	Delay(5);
	IIC_SCL=0;//�X��I2C�`�u�A�ǳƵo�e�α����ƾ� 
}

/**************************��{���********************************************
*��ƭ쫬:		void IIC_Stop(void)
*�\�@�@��:	    //����IIC����H��
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_OUT();//sda�u��X
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	
		Delay(5);
	IIC_SCL=1; 
	IIC_SDA=1;//�o�eI2C�`�u�����H��
	
		Delay(5);							   	
}

/**************************��{���********************************************
*��ƭ쫬:		u8 IIC_Wait_Ack(void)
*�\�@�@��:	    ���������H����� 
//��^�ȡG1�A������������
//        0�A�����������\
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	SDA_IN();      //SDA�]�m����J  
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
	IIC_SCL=0;//������X0  
	return 0;  
} 

/**************************��{���********************************************
*��ƭ쫬:		void IIC_Ack(void)
*�\�@�@��:	    ����ACK����
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
	
/**************************��{���********************************************
*��ƭ쫬:		void IIC_NAck(void)
*�\�@�@��:	    ����NACK����
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

/**************************��{���********************************************
*��ƭ쫬:		void IIC_Send_Byte(u8 txd)
*�\�@�@��:	    IIC�o�e�@�Ӧr�`
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t; 
		SDA_OUT(); 	    
    IIC_SCL=0;//�ԧC�����}�l�ƾڶǿ�
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
   
/**************************��{���********************************************
*��ƭ쫬:		u8 IIC_Read_Byte(unsigned char ack)
*�\�@�@��:	    //Ū1�Ӧr�`�Aack=1�ɡA�o�eACK�Aack=0�A�o�enACK 
*******************************************************************************/  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA�]�m����J
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
        IIC_Ack(); //�o�eACK 
    else
        IIC_NAck();//�o�enACK  
    return receive;
}

/**************************��{���********************************************
*��ƭ쫬:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*�\�@�@��:	    Ū�����w�]�� ���w�H�s���� length�ӭ�
��J	dev  �ؼг]�Ʀa�}
		reg	  �H�s���a�}
		length �nŪ���r�`��
		*data  Ū�X���ƾڱN�n�s�񪺫��w
��^   Ū�X�Ӫ��r�`�ƶq
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev<<1);	   //�o�e�g�R�O
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //�o�e�a�}
  IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte((dev<<1)+1);  //�i�J�����Ҧ�	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //�aACK��Ū�ƾ�
		 	else  data[count]=IIC_Read_Byte(0);	 //�̫�@�Ӧr�`NACK
	}
    IIC_Stop();//���ͤ@�Ӱ������
    return count;
}

/**************************��{���********************************************
*��ƭ쫬:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*�\�@�@��:	    �N�h�Ӧr�`�g�J���w�]�� ���w�H�s��
��J	dev  �ؼг]�Ʀa�}
		reg	  �H�s���a�}
		length �n�g���r�`��
		*data  �N�n�g���ƾڪ����a�}
��^   ��^�O�_���\
*******************************************************************************/ 
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev<<1);	   //�o�e�g�R�O
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //�o�e�a�}
	IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
 }
	IIC_Stop();//���ͤ@�Ӱ������

    return 1; //status == 0;
	
}
