#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "ds1302.h"	

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit S7  = P3^0;
sbit a=P0^2;
sbit beep=P0^6;


void delay(u16 i)
{
	while(i--);	
}

void Display()
{
	u16 i;
	for(i=0;i<=7;i++)
	{
		P2=0xc0;
    switch(i)
		{
			case 0:P0=0x01;break;
			case 1:P0=0x02;break;
			case 2:P0=0x04;break;
			case 3:P0=0x08;break;
			case 4:P0=0x10;break;
			case 5:P0=0x20;break;
			case 6:P0=0x40;break;
			case 7:P0=0x80;break;
		}
		P2=0xe0;
		if(S7==1)               //不摁下显示时间
		{
		P0=DisplayData[i];
		delay(100);	 
		P0=0xff;	
		}
		else									 //摁下显示日历
		{
		P0=DisplayData[i+8];
		delay(100);	 
		P0=0xff;
		}
	}
}

void LED()
{
	int i;
		if(DisplayData[7]==0xF9)  
		{
			P2=0x80;
			for(i=0;i<7;i++)
			{
				P0=(0x80>>i);
				delay(50000);
			}
			P0=0xff;
		}
}

void voice()
{
		if(DisplayData[3]==0x92&&DisplayData[4]==0xF9&&DisplayData[7]==0xC0)
		{
			P2=0xa0;
			beep=1;
			delay(5000);
			beep=0;
		}
}

void main()
{	
	Ds1302Init();	   //第一次初始化后就可以注释该条语句，这样下次重启就不会再次初始化了
	while(1)
	{
		datapros();	 //数据处理函数
		Display();	 //数码管显示函数
		LED();
		voice();
	}		
}

