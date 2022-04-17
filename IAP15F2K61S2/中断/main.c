#include <reg52.H>
#include<interrupt.h>

sbit a=P2^0;

void delay(unsigned int i);
void LED();

void main()
{
	int0();
	int1();
	P2=10001111;
//	a=1;

	while(1)
	{
			P0=0x80;
	}
}

