/*

���աG STM32F103C8T6
�\��G����GY-901�Ҷ��o�e���ƾڡA�åΦ�f���L�X�ѪR�X�Ӫ��ƾڡA�i�S�v9600
		�TTM32F103��f1��Rx��JY-901��Tx,�Ω󱵦�JY-901�Ҷ����ƾ�
			STM32F103��f1��Rx���q����USB-TTL�Ҷ���Rx�A�Ω�V�q���o�e�ѪR�H�᪺�ƾڡC�q���ݳq�L��f�ոէU��d�ݡA�i�S�v9600�AASCII�X�覡��ܡC
			�ƾڸѪR�ϥε��c�骺��k�i��ѪR�C���c��bJY901.h��󤤩w�q

---------�w��W���޸}�s��:----------
TTL���f�G
PC(USB-TTL)			STM32F103						JY-901
RX			<-->	PA9(UART1-TXD)
							PA10(UART1-RXD)	<-->  	TX
------------------------------------
*/
#include "stm32f10x.h"
#include "UARTs.h"
#include "IOI2C.h"
#include "delay.h"
#include "JY901.h"
#include "string.h"
#include "timer.h"
#include "key.h"
#include "led.h"
#include "delaysys.h"
#include "time.h"
#include "24l01.h"
#include "adc.h"
#include <math.h>

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;

int X=0;
int Y=0;
int Z=0;
int count1=0;
int dir=0;
int pcount=0;
int tcount=0;
int zcount=0;
int counter=0;
int tempad[4];
int adkey=0;
float real_ad=0;
float t=0;
float x_angle[5];
float y_angle[5];
float z_angle[5];
float x_acc[5];
float acX=0;
float acZ=0;
float z_acc[5];
float accx[2];
float accz[2];
float offset=0;
float h=0;
float d=0;
float clat=0;
float clon=0;
float p=0;
float temlon[2];
float temlat[2];
float course=0;
float xsum=0;
float ysum=0;
float zsum=0;
float accxsum=0;
float acczsum=0;
float dx=0;
float dz=0;
float w=0;
float b=0;
float x_rudder=0;
float y_rudder=0;
float nose_down=0;
float lontitude=0;
float latitude=0;
float lontitude_home=121.268323;
float latitude_home=24.86670;
float dlontitude=0;
float dlatitude=0;  //�g�n�װ��]
u16 Right_rudder_stand_point=0;//�k��������I
u16 Left_rudder_stand_point=0;//����������I
u16 Right_rudder_uppermost_sheet=0;//�k����̤W�I
u16 Left_rudder_uppermost_sheet=0;//������̤W�I
u16 Right_rudder_lowermost_sheet=0;//�k����̤U�I 
u16 Left_rudder_lowermost_sheet=0;//������̤U�I
u16 ad_data;	 
u16 Right_rudder_actual_value=0;//�k���ڼƭ�
u16 Left_rudder_actual_value=0;//�����ڼƭ�
u16 wind_open;
u16	wind_close;
u16 hook_open;
u16 hook_close;
u16 hook_actual;
u16 wind_actual=552;
float Moment_right_rudder=0;//��U�k��ʤ���p�ƥΩ�B��ϥ�
float Left_rudder_moment=0;//��U��������p�ƥΩ�B��ϥ�
int Moment_right_rudder1=0;//��U�k��ʤ���
int Left_rudder_moment1=0;//��U��������
double azimuth=0;
u8 str[100];
	
float RU_difference;//�P��Ǥ����� �k�V�W�P��Ǯt��
float RD_difference;//�P��Ǥ���W�[  �k�V�U�P��Ǯt��
float LU_difference;//�P��Ǥ���W�[    ���V�W�P��ǭȮt��
float LD_difference;//�P��Ǥ�����    ���V�U�P��ǭȮt��
//CopeSerialData����f���_�եΨ�ơA��f�C����@�ӼƾڡA�եΤ@���o�Ө�ơC
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //�ƾ��Y����A�h���s�}�l�M��0x55�ƾ��Y
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//�ƾڤ���11�ӡA�h��^
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy���sĶ���۱a�����s������ơA�ݤޥ�"string.h"�A�N�����w�İϪ��r�ū�����ƾڦ@�P��̭��A�q�ӹ�{�ƾڪ��ѪR�C
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}
void Up_and_down_control(int Up_and_down_Right,int Up_and_down_Left);//�k�̤j��+100,-100	 ���̤j��+100,-100
void Fly_fly_left_and_right(int Right_and_left_offset);//+100�k  -100��
void The_actual_motor_pwm(void);//�p���ڭn��pwmp
void TIM3_IRQHandler(void);
void fly_option(void);
void wind_up_down(void);
void wireless(int ad);
void position(void);//��쨤�p��
void height(float pressure);//calculate the height
void airspeed(int ad_data);


int main(void)
{
	delay_init1();
  SystemInit();	/* �t�m�t�ή�����72M �ϥΥ~��8M����+PLL*/ 
	SysTick_init(72,10);		//���ɪ�l��
	Adc_Init();	
	LED_Init();	
	Initial_UART1(9600);
	delay_ms1(1000);//����JY-91��l�Ƨ���?
	TIM3_Int_Init(99,7199);
	TIM4_PWM_Init(4999,195);	 //��196�ᰣ5000==73.46938775510204hz
	//NRF24L01_Init();    		//2.4g��l��
  //NRF24L01_TX_Mode();     //�o�e��ƼҦ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //�]�mNVIC���_����2:2��m�e�u���šA2���T���u����

	Right_rudder_stand_point=514;//�k�_�l�����I
	Right_rudder_uppermost_sheet=366;//�k�̤W�I
	Right_rudder_lowermost_sheet=633;//�k�̤U�I
	Left_rudder_stand_point=551;//���_�l�����I
  Left_rudder_uppermost_sheet=717;//���̤W�I
	Left_rudder_lowermost_sheet=422;//���̤U�I
	wind_open=459;
	wind_close=552;
	hook_open=1986;
	hook_close=961;
  RU_difference=Right_rudder_stand_point-Right_rudder_uppermost_sheet;//�P��Ǥ����� �k�V�W�P��Ǯt��
	RD_difference=Right_rudder_lowermost_sheet-Right_rudder_stand_point;//�P��Ǥ���W�[  �k�V�U�P��Ǯt��
	LU_difference=Left_rudder_uppermost_sheet-Left_rudder_stand_point;//�P��Ǥ���W�[    ���V�W�P��ǭȮt��
	LD_difference=Left_rudder_stand_point-Left_rudder_lowermost_sheet;//�P��Ǥ�����    ���V�U�P��ǭȮt��
	
	while(1)
	{	
		X=(float)stcAngle.Angle[0]/32768*180;
		Y=(float)stcAngle.Angle[1]/32768*180;
		Z=(float)stcAngle.Angle[2]/32768*180;
		//h=(float)stcPress.lAltitude;
		acX=(float)stcAcc.a[0]/32768*16;
		acZ=(float)stcAcc.a[2]/32768*16;
		p=stcPress.lPressure/100;
		ad_data=Get_Adc_Average(ADC_Channel_4,10);  //250ms/��
		Moment_right_rudder1=0;//��U�k��ʤ���
    Left_rudder_moment1=0;//��U����ʤ���
	if(adkey==1)
		{
			airspeed(ad_data);
//		if(h<=1010 && h>=990)
//			{                                       //��_�[�t���q
//				x_rudder=0;
//				y_rudder=0;
//				hook_actual=hook_open;
//			}
		if(h<=5 && real_ad>=80)
			{                                       //landing
//				wind_actual=459;
				if(y_rudder<0)
				wind_up_down();
				hook_actual=hook_close;
			}
			if(real_ad<80)
			{
				y_rudder=80-real_ad-ysum;
			}
//		if(h<=990 && h>5 && real_ad<=60)
//			{
//				hook_actual=hook_close;                //stall
//				y_rudder=-(90-ysum);
//			}
//		if(real_ad>=60 && h>5){
//				hook_actual=hook_close;
//				if(real_ad<70)                                                    //�ųt
//				{
//					y_rudder=-((70-real_ad)-y_rudder);                              //�ųt�ܤ֤j��80
			}
//			if(y_rudder>0 && real_ad>70)
//				{
//					wind_up_down();
//				}
			dx=accx[1]-accx[0];
			dz=accz[1]-accz[0];
		LED0=!LED0;
			if(pcount>=85)                     //navigation
		{
			//position();
			pcount=0;
		}
//	}//depart from ballon and speed is enough
//	}
		if(x_rudder>100)x_rudder=100;
		if(y_rudder>100)y_rudder=100;
		if(x_rudder<-100)x_rudder=-100;
		if(y_rudder<-100)y_rudder=-100;
		Up_and_down_control(-y_rudder,-y_rudder);
		Fly_fly_left_and_right(x_rudder);
		The_actual_motor_pwm();
		TIM_SetCompare1(TIM4,hook_actual);
		TIM_SetCompare2(TIM4,wind_actual);
		TIM_SetCompare3(TIM4,Right_rudder_actual_value);//�k��pwm
		TIM_SetCompare4(TIM4,Left_rudder_actual_value);//����pwm	
		TIM3_IRQHandler();
	}//while loop end
}//�D�`�� end 


void Up_and_down_control(int Up_and_down_Right,int Up_and_down_Left)//�k+100 -100 ��+100 -100
{

	
	if(Up_and_down_Right>0)
	{
		Moment_right_rudder1 = Up_and_down_Right+Moment_right_rudder1;
		if(Moment_right_rudder1>100)
		{
			Moment_right_rudder1=100;
		}
		
	}
	else if(Up_and_down_Right<0)
	{
		Moment_right_rudder1=Up_and_down_Right+Moment_right_rudder1;
		if(Moment_right_rudder1<-100)
		{
			Moment_right_rudder1=-100;	
		}
	}
	
	
	
	if(Up_and_down_Left>0)
	{
		Left_rudder_moment1=Up_and_down_Left+Left_rudder_moment1;
		if(Left_rudder_moment1>100)
		{
			Left_rudder_moment1=100;
		}
	}
	else if(Up_and_down_Left<0)
	{
		Left_rudder_moment1=Up_and_down_Left+Left_rudder_moment1;
		if(Left_rudder_moment1<-100)
		{
			Left_rudder_moment1=-100;
		}
	}
		
	
}

void Fly_fly_left_and_right(int Right_and_left_offset)//100�k��-100����
{
	if(Right_and_left_offset>0)//�k��
	{
		Moment_right_rudder1=Moment_right_rudder1+Right_and_left_offset;
		if(Moment_right_rudder1>100)
		{
			Moment_right_rudder1=100;
		}
		Left_rudder_moment1=Left_rudder_moment1-Right_and_left_offset;
		if(Left_rudder_moment1<-100)
		{
			Left_rudder_moment1=-100;
		}
		
	}
	else if(Right_and_left_offset<0)//����
	{
		Moment_right_rudder1=Right_and_left_offset+Moment_right_rudder1;
		if(Moment_right_rudder1<-100)
		{
			Moment_right_rudder1=-100;
		}
		Left_rudder_moment1=Left_rudder_moment1-Right_and_left_offset;
		if(Left_rudder_moment1>100)
		{
			Left_rudder_moment1=100;
		}
		
	}
	
}
void The_actual_motor_pwm(void)
{
	if(Moment_right_rudder1>0)
	{
		Moment_right_rudder=Moment_right_rudder1;
		Moment_right_rudder=Moment_right_rudder/100;
		Right_rudder_actual_value=Right_rudder_stand_point-(u16)(Moment_right_rudder*RU_difference);
		
	}
	else if(Moment_right_rudder1<0)
	{
		Moment_right_rudder=-Moment_right_rudder1;
		Moment_right_rudder=Moment_right_rudder/100;
		Right_rudder_actual_value=Right_rudder_stand_point+(u16)(Moment_right_rudder*RD_difference);
	}
	
	
	if(Left_rudder_moment1>0)
	{
		Left_rudder_moment=Left_rudder_moment1;
		Left_rudder_moment=Left_rudder_moment/100;
		Left_rudder_actual_value=Left_rudder_stand_point+(u16)(Left_rudder_moment*LU_difference);
		
	}
	else if(Left_rudder_moment1<0)
	{
		Left_rudder_moment=-Left_rudder_moment1;
		Left_rudder_moment=Left_rudder_moment/100;
		Left_rudder_actual_value=Left_rudder_stand_point-(u16)(Left_rudder_moment*LD_difference);
	}
	
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
	{
	pcount++;
	x_angle[dir]=X;
	y_angle[dir]=Y;
	z_angle[dir]=Z;
	x_acc[dir]=acX;
	z_acc[dir]=acZ;
	dir++;
		if(dir==4)
		{
			for(dir=0;dir<5;dir++)
			{
				xsum=xsum+x_angle[dir];
				ysum=ysum+y_angle[dir];
				zsum=zsum+z_angle[dir];
				accxsum=accxsum+x_acc[dir];
				acczsum=acczsum+z_acc[dir];
				x_angle[dir]=0;
				y_angle[dir]=0;
				z_angle[dir]=0;
				x_acc[dir]=0;
				z_acc[dir]=0;
			}
			xsum=xsum/5;
			ysum=ysum/5;
			zsum=zsum/5;
			y_rudder=-ysum*2.3;
			ysum=0;
			if(real_ad>=80)
			{
				x_rudder=-xsum*2.3;
				xsum=0;
//				if(offset>45)offset=45;
//				if(offset<-45)offset=-45;
//				x_rudder=x_rudder+offset*1.1;
//				if(offset>20 || offset<-20)y_rudder=-(y_rudder-10);
			}
			accxsum=accxsum/5;
			acczsum=acczsum/5;
			accx[count1]=accxsum;
			accz[count1]=acczsum;
			dir=0;
			if(count1==1)count1=0;
			else count1++;
		}
		if(counter>=500)
		{
			tcount++;                
			wireless(ad_data);
		}
		//height(stcPress.lPressure);
	
		if(adkey==0)
		{
			counter++;                   //count 5 seconds
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
	}
}

void fly_option(void)
{
	//if(dx==0 && dz==0)
	//{
		//x_rudder=x_rudder+50;
	//}
	//if(dx==0 && dz<-0.06)
	//{
	//	x_rudder=x_rudder+((-dz)*1000);
	//}
	//if(dx==0 && dz>0.06)
	//{
	//	x_rudder=x_rudder+(dz*1000);
	//}
	//if(dx<-0.04 && dz==0)
	//{
	//	x_rudder=x_rudder+((-dx)*1000);
	//}
	//if(dx<-0.04 && dz<-0.08)
	//{
	//	x_rudder=x_rudder+((-dz)*1000);
	//}
	//if(dx<-0.04 && dz>0.06)
	//{
	//	x_rudder=x_rudder+(dz*1000);
	//}
	//if(dx>0.04 && dz==0)
	//{
	//	x_rudder=x_rudder-(dx*1000);
	//}
	//if(dx>0.04 && dz>0.06)
	//{
	//	x_rudder=x_rudder-(dz*1000);
	//}
	//flag=0;
}
void wind_up_down(void)
{
	wind_actual=wind_close+(wind_close-wind_open)*1.5/100*y_rudder;
	if(wind_actual>=552)wind_actual=552;
	if(wind_actual<=459)wind_actual=459;
}

void position(void)
{
	lontitude=((double)stcLonLat.lLon/10000000+(double)(stcLonLat.lLon % 10000000)/1e5/60)*3.1415926/180;//�{�b�g��
	latitude=((double)stcLonLat.lLat/10000000+(double)(stcLonLat.lLat % 10000000)/1e5/60)*3.1415926/180;//�{�b�n��
	dlontitude=lontitude_home-lontitude;
	dlatitude=latitude_home-latitude;
	azimuth=atan((dlontitude/dlatitude));
	azimuth=azimuth*180/3.1415926;
	if(dlontitude<0 && dlatitude>0)azimuth=360+azimuth;
	if(dlontitude<0 && dlatitude<0)azimuth=360-azimuth;
	if(dlontitude>0 && dlatitude<0)azimuth=180+azimuth;
	if(zcount==1)
	{
		temlon[0]=lontitude;
		temlat[0]=latitude;
	}
	if(zcount==3)
	{
		temlon[1]=lontitude;
		temlat[1]=latitude;
	}
	if(temlon[1]!=0 && temlat[1]!=0)                             //�p���ɧέ���
	{
		dlontitude=temlon[0]-temlon[1];
		dlatitude=temlat[0]-temlat[1];
		course=atan((dlontitude/dlatitude));
		course=course*180/3.1415926;
		if(dlontitude<0 && dlatitude>0)course=360+course;
		if(dlontitude<0 && dlatitude<0)course=360-course;
		if(dlontitude>0 && dlatitude<0)course=180+course;
	}
	//course=0;
	offset=azimuth-course;
	if(offset>180)
	{
		offset=offset-360;
	}
	if(offset<-180)
	{
		offset=360+offset;
	}
	clat=dlatitude*110.574*dlatitude*110.574;
	clon=dlontitude* 111.320*cos(latitude)*dlontitude* 111.320*cos(latitude);
	d=sqrt((clat+clon));
	nose_down=atan(h/d);
	y_rudder=-(y_rudder-nose_down);
	zcount++;
	if(zcount==5)
	{
	  temlon[0]=temlon[1];
		temlon[1]=0;
		temlat[0]=temlat[1];
		temlat[1]=0;
		zcount=0;
	}
}
void wireless(int ad)
{
	if(tcount==2)
		{
			UART1_Put_Char(0x66);		//�}�Y
		}
		if(tcount==19)
		{
			UART1_Put_Char(stcPress.lPressure);		//�o�e������Ƶ��j��q��
		}
		if(adkey==0){
			if(tcount==24 || tcount==49 || tcount==74 || tcount==99)
			{
				tempad[((tcount+1)/25)-1]=ad;
				if(tempad[3]!=0)
				{
					tempad[0]=tempad[0]+tempad[1];
					tempad[0]=tempad[0]+tempad[2];
					tempad[0]=tempad[0]+tempad[3];
					tempad[0]=tempad[0]/4;
					adkey=1;
				}
			}

		}
		if(tcount==39){
			UART1_Put_Char(h);	  //�o�e���׸�Ƶ��j��q��
		}
			if(tcount==59){
			UART1_Put_Char(stcLonLat.lLon);
		}
		if(tcount==79){
			UART1_Put_Char(stcLonLat.lLat);
		}
		if(tcount==99){
			UART1_Put_Char(0x64);
			tcount=0;
		}
	//sprintf(str,"1.RL:%u;LL:%uZH:%u",Right_rudder_actual_value,Left_rudder_actual_value,wind_actual);
   // if(NRF24L01_TxPacket(str)==TX_OK)
	 // {
	//		delay_us1(310);
	 // }
		//sprintf(str,"2.Right:%d;Left:%d     ",Moment_right_rudder1,Left_rudder_moment1);
    //if(NRF24L01_TxPacket(str)==TX_OK)
	  //{
		//	delay_us1(520);
	  //}
 	//sprintf(str,"3.Acc:%.2f %.2f %.2f",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
   //if(NRF24L01_TxPacket(str)==TX_OK)
  //{
	//		delay_us1(340);
	//	}
   // sprintf(str,"4.Angle:%.2f %.2f %.2f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
  	//if(NRF24L01_TxPacket(str)==TX_OK)
  // {
	//		delay_us1(570);
  //  }
	//	sprintf(str,"5.Airspeed:%u",ad_data);
// 	if(NRF24L01_TxPacket(str)==TX_OK)
 //  {
		//	delay_us1(390);
    //}
		//sprintf(str,"6.GPH:%.1fm GPY:%.1fDeg",(float)stcGPSV.sGPSHeight/10,(float)stcGPSV.sGPSYaw/10,(float)stcGPSV.lGPSVelocity/1000);
		//if(NRF24L01_TxPacket(str)==TX_OK)
  // {
	//		delay_us1(530);
   // }
	 // sprintf(str,"7.GPV:%.3fkm",(float)stcGPSV.lGPSVelocity/1000);
	  //if(NRF24L01_TxPacket(str)==TX_OK)
  // {
	//	delay_us1(520);
   // }
    //sprintf(str,"8.N:%ldD%.5f E:%ldD%.5f",stcLonLat.lLon/10000000,(double)(stcLonLat.lLon % 10000000)/1e5,stcLonLat.lLat/10000000,(double)(stcLonLat.lLat % 10000000)/1e5);
		//if(NRF24L01_TxPacket(str)==TX_OK)
   // {
		//	delay_us1(360);
    //}	
		//sprintf(str,"9.T:20%d-%d-%d %d:%d:%.3f",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		//if(NRF24L01_TxPacket(str)==TX_OK)
  // {
	//		delay_us1(370);
   //}			
}
void height(float pressure)
{
	w=1-(pressure/p);
	b=9.80665*1.225*t/(0.0065*pressure);
	w=pow(w,b);
	h=t/6.5*w;
}

void airspeed(int ad)
{
	real_ad=(ad-tempad[0])*10/15;
	if(real_ad<0)real_ad=0;
}

