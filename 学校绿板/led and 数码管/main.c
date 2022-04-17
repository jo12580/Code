#include <REGX52.H>

sbit led1=P3^3;
sbit led2=P3^4;

void led();
void shu();
void delay(unsigned int i);

void main()
{		
	while(1)
	{
//		shu();
		P0=~P0;
		delay(50000);
	}
}

void led()
{
	led1=!led1;
	delay(50000);
	led2=!led2;
	delay(5000);
}

void shu()          //¹²Ñô
{
	unsigned int i,a[18]={ 0x18, 0xDE, 0x34, 0x94, 0xD2, 0x91, 0x11, 0xDC,    
					0x10, 0x90, 0x50, 0x13, 0x39, 0x16, 0x31, 0x71, 0xF7, 0xEF };
	for(i=0;i<=18;i++)
	{
		P0=a[i];
		delay(50000);
		led();
	}
}

void delay(unsigned int i)
{
	while(i--);
}