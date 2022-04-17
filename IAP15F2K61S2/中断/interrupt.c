#include <REGX52.H>

void delay(unsigned int i)
{
	while(i--);
}

void int0()		//判断3^2引脚
{
	EA=1;
	EX0=1;
	IT0=1;		//置1时为下降沿触发、置0时为低电平触发
}

void int1()		//判断3^3引脚	
{
	EA=1;
	EX1=1;
	IT1=1;		
}

void int0_intfun() interrupt 0 using 1//输入中断执行的程序
{
	unsigned int i;
	for(i=0;i<7;i++)
	{
	P0=(0xfe<<i);
			delay(50000);
		delay(50000);
		delay(50000);
	}
													
}

void int1_intfun() interrupt 2 using 1	//输入中断执行的程序
{
		unsigned int i;
		for(i=0;i<7;i++)
	{
		P0=(0xbf>>i);
		delay(50000);
		delay(50000);
		delay(50000);
	}
																	
}