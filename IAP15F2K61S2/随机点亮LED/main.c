#include <reg52.H>
#include<stdlib.h>

unsigned char led1[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//灭某个
unsigned char led2[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//亮某个

void delay(unsigned int i);
void LED();

void main()
{
	P2=0x80;
	while(1)
	{
		delay(50000);
		delay(50000);
		LED();
		delay(50000);
		delay(50000);
	}
}

void delay(unsigned int i)
{
	while(i--);
}

void LED()
{
//	unsigned int num=rand()%8;
//	P0=led2[num];
																unsigned int num=rand()%512;
															P0=num;
												/*第二种方法：
															unsigned int num=rand()%(n-m+1)+m;
															P0=num;
												*/
	
}