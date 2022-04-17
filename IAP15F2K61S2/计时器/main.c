#include <reg52.H>
#include<stdlib.h>

void Timer0Init(void)		
{
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA=1;
	ET0=1;
}

void main()
{
	Timer0Init();
	P2=0x80;
	while(1);
		
}

void Timer0() interrupt 1
{
	static unsigned int i;
	unsigned int num;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	i++;
	num=rand()%512;
	if(i==1000)
	{
		i=0;
		P0=num;
	}
}