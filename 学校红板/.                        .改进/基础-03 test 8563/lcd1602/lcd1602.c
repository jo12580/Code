#include "lcd1602.h"
#include "delay.h"
#include <reg52.h>

//sbit rs=P1^0;
//sbit rw=P1^1;
//sbit en=P1^2;

void write_com(uchar com)
{
	uchar temp;
	
	en=0;
	rs=0;
	rw=0;
	
	temp=com&0xf0;
	P1&=0x0f;
	P1|=temp;
	en=1;
	en=0;

	temp=(com<<4)&0xf0;
	P1&=0x0f;
	P1|=temp;
	en=1;
	en=0;
}

void readbusy(void)
{
	P1|=0x80;  //为读取BF信号作准备
	rw=1;
	rs=0;
	en=1;
	while(P1&0x80);	 //等待
	en=0;
}

void init_lcd1602(void)
{
	delay15ms();
	write_com(0x28);
	delay5ms();
	write_com(0x28);
	delay5ms();
	write_com(0x28);

	readbusy();
	write_com(0x28);
	readbusy();
	write_com(0x08);
	readbusy();
	write_com(0x01);
	readbusy();
	write_com(0x06);
	readbusy();
	write_com(0x0c);
}

void locate(uchar x,uchar y)
{
	uchar ac;
	if(y==0) ac=0x80+x;
	else     ac=0xc0+x;
	readbusy();
	write_com(ac);
}

void display_str(uchar *str)
{
 	uint i=0;

	while(str[i]!='\0')
	{
		readbusy();
		write_data(str[i]);
		i++;
	}
}

void write_data(uchar dat)
{
	uchar temp;
	
	en=0;
	rs=1;
	rw=0;
	
	temp=dat&0xf0;
	P1&=0x0f;
	P1|=temp;
	en=1;
	en=0;

	temp=(dat<<4)&0xf0;
	P1&=0x0f;
	P1|=temp;
	en=1;
	en=0;
}

