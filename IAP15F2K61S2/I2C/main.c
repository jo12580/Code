#include <reg52.H>
#include<i2c.h>

sbit S7  = P3^0;
sbit S6  = P3^1;
sbit S5  = P3^2;
sbit S4  = P3^3;

unsigned int num;

void delay(unsigned int i )
{
	while(i--);
}

void main()
{
	P2=0x80;
	while(1)
	{
		if(S7==0)
		{
				delay(5000);
				if(S7==0)
				{
					num=0;
					At24c02Write(1, num);			//写入
				}
				while(!S7);
		}
		
				if(S6==0)
		{
				delay(5000);
				if(S6==0)
				{
					At24c02Read(1);					//读出
					P0=num;
				}
				while(!S6);
		}
		
						if(S5==0)
		{
				delay(5000);
				if(S5==0)
				{
					num++;									//累加
				}
				while(!S5);
		}
		
						if(S4==0)
		{
				delay(5000);
				if(S4==0)
				{
						num=0;										//清零
				}
				while(!S4);
		}
		
		
	}
	
}