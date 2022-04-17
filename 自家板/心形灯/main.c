#include <REGX52.H>

void Delay1ms()	;

void main()
{
	while(1)
	{
		P0=0x78;
		P1=0x00;
		P2=0x00;
		P3=0x00;
		Delay1ms();
		P0=0xff;
		P1=0xff;
		P2=0xff;
		P3=0xff;
		Delay1ms();
	}
}

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

