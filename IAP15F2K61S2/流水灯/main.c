#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

int i;

void delay(u16 i)
{
	while(i--);	
}														  /*
														  P2=0XA0;//�򿪿��Ʒ�������573
														P0=0X00;//�رշ������̵���
														*/


void main(void)
{
	P2=0x80;     //��	P2^7Ϊһ��ʹ74HC573оƬ����
	while(1)
	{
for(i=0;i<7;i++)
	{
	P0=(0xfe<<i);
			delay(50000);
		delay(50000);
		delay(50000);
	}
delay(50000);
	for(i=0;i<7;i++)
	{
	P0=(0xbf>>i);
			delay(50000);
		delay(50000);
		delay(50000);
	}
	delay(50000);
}
}
