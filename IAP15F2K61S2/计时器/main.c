#include <reg52.H>
#include<stdlib.h>

void Timer0Init(void)		
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	i++;
	num=rand()%512;
	if(i==1000)
	{
		i=0;
		P0=num;
	}
}