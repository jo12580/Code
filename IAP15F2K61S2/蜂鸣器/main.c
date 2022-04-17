#include <reg52.H>
#include<stdlib.h>

sbit a=P0^6;

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void delay(unsigned int i)
{
	while(i--);
}

void main()
{
	unsigned int i;
	P2=0xa0;
	while(1)
	{
		i=rand()%100+2000;
			a=~a;
		delay(i);	 
	}
}
