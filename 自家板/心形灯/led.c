#include <STC15F2K60S2.H>
#include<led.h>
#include<stdlib.h>

void Clockwise()
{
	unsigned int i;
	for(i=0;i<=16;i++)
	{
		P2=(0000100011111111>>i);
		if(i>=8)
		{
				P3=(0000100011111111>>(i-8));
		}
		delay(1000);
	}
	
		for(i=0;i<=8;i++)
	{
		P3=(00001000>>i);
		P1=(0000100011111111>>i);
		delay(1000);
	}	
	
		for(i=0;i<=8;i++)
	{
		P1=P1>>i;	
		P0=(11111111)<<(00001000);
		delay(1000);
	}
	
		for(i=0;i<=8;i++)
	{
		P0=P0<<i;
		delay(1000);
	}
}

//void anti_clockwise()
//{
//	unsigned int i;
//	for(i=0;i<=8;i++)
//	{
//		P0=(1100000000>>i);
//		delay(1000);
//	}
//}

//void twinkle()
//{
//	unsigned int i=3;
//	while(i--)
//	{
//	P0=0x00;P1=0x00;P2=0x00;P3=0x00;		//¿ª
//	delay(1000);
//  P0=0xff;P1=0xff;P2=0xff;P3=0xff;	  //Ï¨
//	delay(10000);delay(10000);delay(10000);
//	
//	P0=0x00;P1=0x00;P2=0x00;P3=0x00;		//¿ª
//	delay(1000);
//  P0=0xff;P1=0xff;P2=0xff;P3=0xff;		//Ï¨
//	delay(1000);
//	P0=0x00;P1=0x00;P2=0x00;P3=0x00;		//¿ª
//	delay(1000);
//  P0=0xff;P1=0xff;P2=0xff;P3=0xff;		//Ï¨
//	delay(1000);
//	}
//}

//void random()
//{
//	unsigned int i=10,num;
//	while(i--)
//	{
//		num=rand()%257;
//		P0=num;P1=num+11;P2=num+21;P3=num+31;
//		delay(11000);
//	}
//}

//void flash()
//{
//	while(1)
//	{
//		P0=0x00;P1=0x00;P2=0x00;P3=0x00;
//	}
//}

void delay(unsigned int i)
{
	while(i--);
}