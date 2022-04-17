#include "reg51.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include<intrins.h>

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;
sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;
u8 ledduan[]={0x30,0x48,0x44,0x22,0x22,0x44,0x48,0x30};            //D	��	
u8 ou[]={0xea,0xae,0xea,0x00,0x82,0x61,0x8f,0x00};
u8 xt[]={0xa0,0x40,0xa8,0x0e,0x88,0xe0,0x80,0x00};
u8 xl[]={0x00,0x30,0x34,0x02,0x02,0x34,0x30,0x00}  ;
				   
u8 ledwei[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};			   //P	��	 (�ӵ���������ߵ�ѹ)
/*******************************************************************************
* �� �� ��         : delay
* ��������		   : ��ʱ������i=1ʱ����Լ��ʱ10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* ������         : Hc595SendByte(u8 dat)
* ��������		   : ��74HC595����һ���ֽڵ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Hc595SendByte(u8 dat)
{											   
	u8 a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();					   //�ӳ�
	_nop_();
	RCLK=0;
}

void main()
  {      	while (1)  {
				if(k2==0)//��							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //λѡ
			Hc595SendByte(ledduan[i]);	//���Ͷ�ѡ����
			delay(100);		   //��ʱ
			Hc595SendByte(0x00);  //����
		}	
	}		
}
  






  			if(k1==0)	//oyzj						 
{    u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //λѡ
			Hc595SendByte(ou[i]);	//���Ͷ�ѡ����
			delay(100);		   //��ʱ
			Hc595SendByte(0x00);  //����
		}	
	}		
}
 





	if(k3==0)							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //λѡ
			Hc595SendByte(xt[i]);	//���Ͷ�ѡ����
			delay(100);		   //��ʱ
			Hc595SendByte(0x00);  //����
		}	
	}		

  }




if(k4==0)							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //λѡ
			Hc595SendByte(xl[i]);	//���Ͷ�ѡ����
			delay(100);		   //��ʱ
			Hc595SendByte(0x00);  //����
		}	
	}		
}
}
}
