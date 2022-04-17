#include <STC15F2K60S2.H>

unsigned int smgduan[19]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 
0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码
unsigned char DisplayData[8];
static unsigned int a=10000,b=0;

void datapros();
void display();

void delay(unsigned int c)
{
	while(c--);
}

void Timer0Init()		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA=1;
	ET0=1;
}

void main()
{
	Timer0Init();
	while(1)
	{
		display();
		delay(100);
	}
}

void Timer0() interrupt 1
{
	unsigned int i;
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	i++;
	if(i==1000)
	{
		a--;
		b++;
		i=0;
	}
}

void datapros() 	 //时间读取处理转换函数
{
	DisplayData[0]=smgduan[a/1000%10];
	DisplayData[1]=smgduan[a/100%10];
	DisplayData[2]=smgduan[a/10%10];
	DisplayData[3]=smgduan[a%10];
	
	DisplayData[4]=smgduan[b/1000%10];
	DisplayData[5]=smgduan[b/100%10];
	DisplayData[6]=smgduan[b/10%10];
	DisplayData[7]=smgduan[b%10];
}

void display()
{
	unsigned int i;
	datapros() ;
	for(i=0;i<=7;i++)
	{
		P2=0xc0;
    switch(i)
		{
			case 0:P0=0x01;break;
			case 1:P0=0x02;break;
			case 2:P0=0x04;break;
			case 3:P0=0x08;break;
			case 4:P0=0x10;break;
			case 5:P0=0x20;break;
			case 6:P0=0x40;break;
			case 7:P0=0x80;break;
		}
		P2=0xe0;
		P0= DisplayData[i];
		delay(100);	
		P0=0xff;
	}
}