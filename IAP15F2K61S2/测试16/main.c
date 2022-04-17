#include "stc15f2k60s2.h"

void new();
unsigned char key();
void delay(unsigned int i);
void Delay10ms()	;

void main()
{	
	unsigned int S,i;
	new();
	while(1)
	{
		S=key();
		if(S==7)
		{
			i=1;
			while(!S);
		}
		if(i==1)
		{
			i=0;
			P2=(P2&0x1f|0x80);
			P0=0x11;
		}
	}
}

void new()
{
	P2 =(P2&0x1F|0x80);
	P0=0xff;
	P2 = (P2&0x1F|0xa0);
	P0=0X00;
	P2 = (P2&0x1F|0xc0);
	P0=0x00;
}

unsigned char key()
{
	unsigned int S;
	P44=0;P42=1;P3=0X7F;
	if(P3!=0X7F)
	{
		Delay10ms();
		P44=0;P42=1;P3=0X7F;
		if(P3!=0X7F)
		{		
			switch(P3)
			{
				case 0x7e:S=7;break;
				case 0x7d:S=6;break;
				case 0x7b:S=5;break;
				case 0x77:S=4;break;
			}
			while(P3!=0X7F)P3=0X7F;
		}
	}
	
	P44=1;P42=0;P3=0XbF;
	if(P3!=0XbF)
	{
		Delay10ms();
		P44=1;P42=0;P3=0XbF;
		if(P3!=0XbF)
		{
			switch(P3)
			{
				case 0xbe:S=11;break;
				case 0xbd:S=10;break;
				case 0xbb:S=9;break;
				case 0xb7:S=8;break;
			}
			while(P3!=0XbF)P3=0XbF;
		}
	}
	
	P44=1;P42=1;P3=0XdF;
	if(P3!=0XdF)
	{
		Delay10ms();
		P44=0;P42=1;P3=0XbF;
		if(P3!=0XdF)
		{
			switch(P3)
			{
				case 0xde:S=15;break;
				case 0xdd:S=14;break;
				case 0xdb:S=13;break;
				case 0xd7:S=12;break;
			}
			while(!P3);
		}
	}
	
	P44=1;P42=1;P3=0XeF;
	if(P3!=0XeF)
	{
		Delay10ms();
		P44=0;P42=1;P3=0XeF;
		if(P3!=0XeF)
		{
			switch(P3)
			{
				case 0xee:S=19;break;
				case 0xed:S=18;break;
				case 0xeb:S=17;break;
				case 0xe7:S=16;break;
			}
			while(!P3);
		}
	}
		return S;
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void delay(unsigned int i)
{
	while(i--);
}