#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

int i;

void delay(u16 i)
{
	while(i--);	
}														  /*
														  P2=0XA0;//打开控制蜂鸣器的573
														P0=0X00;//关闭蜂鸣器继电器
														*/


void main(void)
{
	P2=0x80;     //令	P2^7为一，使74HC573芯片可用
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
