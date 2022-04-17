#include <STC15F2K60S2.H>
#include<Matrix_key.h>

void Delay_key()		//@11.0592MHz  100us
{
	unsigned char i, j;
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}

unsigned char key_scanf()
{
	unsigned int S;
	unsigned char a;
	P44=0;P42=1;P3=0x7f;
	a=P3&0x0f;
	if(a!=0x7f)
	{
		Delay_key();
		a=P3&0x0f;
		if(a!=0x7f)
		{
			switch(P3)
			{
				case 0x77:S=4;break;
				case 0x7b:S=5;break;
				case 0x7d:S=6;break;
				case 0x7e:S=7;break;
			}
			while(!P3);
		}
	}
	
	P44=1;P42=0;P3=0xbf;
	a=P3&0x0f;
	if(a!=0xbf)
	{
		Delay_key();
		if(a!=0xbf)
		{
			switch(P3)
			{
				case 0xb7:S=8;break;
				case 0xbb:S=9;break;
				case 0xbd:S=10;break;
				case 0xbe:S=11;break;
			}
			while(!P3);
		}
	}
	
	P44=1;P42=1;P3=0xdf;
	a=P3&0x0f;
	if(a!=0xdf)
	{
		Delay_key();
		if(a!=0xdf)
		{
			switch(P3)
			{
				case 0xd7:S=12;break;
				case 0xdb:S=13;break;
				case 0xdd:S=14;break;
				case 0xde:S=15;break;
			}
			while(!P3);
		}
	}
	
	P44=1;P42=1;P3=0xef;
	a=P3&0x0f;
	if(a!=0xef)
	{
		Delay_key();
		if(a!=0xef)
		{
			switch(P3)
			{
				case 0xe7:S=16;break;
				case 0xeb:S=17;break;
				case 0xed:S=18;break;
				case 0xee:S=19;break;
			}
			while(!P3);
		}
	}	
	return S;
}


