#include "reg51.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include"LED dot.h"
#include<intrins.h>

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;

void delay(int i)
{
	while(i--);	
}

/*******************************************************************************
* ������         : Hc595SendByte(u8 dat)
* ��������		   : ��74HC595����һ���ֽڵ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/


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
