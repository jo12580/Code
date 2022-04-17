# include<reg52.h>
#include<voice.h>
sbit k1=P1^6;  //语音播放引脚定义 
sbit k2=P1^7;  //语音播放引脚定义 
sbit Led1=P3^3;
sbit Led2=P3^4;

void key();
void  delay(unsigned int i);

unsigned int i;

void main()
{
	Led1=0; Led2=1;delay(100);	
//	while (1)
//	{
		key();
		voice(i);
		delay(40000);
//		k1=1;
//		k2=1;
		Led1=~Led1;  	
		Led2=~Led2;		
//	}
}

void key()
{
	unsigned int a;
	P0=0;
	if(P0!=0x00)
	{
		delay(1000);
		P0=0;
		if(P0!=0x00)
		{
			a=P0;
			switch(a)
			{
				case 0x01:i=2;break;
				case 0x02:i=3;break;
				case 0x04:i=4;break;
				case 0x08:i=5;break;
				case 0x10:i=6;break;
				case 0x20:i=7;break;
				case 0x40:i=8;break;
				default: 0xff;
			}
		}
	}
}
 

void  delay(unsigned int i)
{
	while(i--);
}
