#include<reg52.h>
#include "absacc.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
int a=100, i;

sbit S7  = P3^0;
sbit S6  = P3^1;
sbit S5  = P3^2;
sbit S4  = P3^3;

sbit c=	P3^7;

void delay(u8 i)
{
while(i--);

}

void LED()
{
			while(a--)
			{
				for(i=0;i<7;i++)
				{
					P0=(0xfe<<i);
					delay(50000);
				}
					delay(50000);
				for(i=0;i<7;i++)
				{
					P0=(0xfe>>i);
					delay(50000);
				}
			}
}

void main()
{
		c=0;
	while(1)
	{
			P2=0x80;
		
		if(S7==0)			//开灯
		{
			delay(5000);
			if(S7==0)
			{
				P0=0x00;
			}
			while(!S7);
		}
		
		if(S6==0)      //熄灯
		{
			delay(5000);
			if(S6==0)  
			{
				P0=0xff;
			}
			while(!S6);
		}
		
		if(S5==0)
		{
			delay(5000);
			if(S5==0)
			{
				LED();
			}
			while(!S5);
		}
		
		
	}
		
}
