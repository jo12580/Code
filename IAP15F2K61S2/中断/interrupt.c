#include <REGX52.H>

void delay(unsigned int i)
{
	while(i--);
}

void int0()		//�ж�3^2����
{
	EA=1;
	EX0=1;
	IT0=1;		//��1ʱΪ�½��ش�������0ʱΪ�͵�ƽ����
}

void int1()		//�ж�3^3����	
{
	EA=1;
	EX1=1;
	IT1=1;		
}

void int0_intfun() interrupt 0 using 1//�����ж�ִ�еĳ���
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

void int1_intfun() interrupt 2 using 1	//�����ж�ִ�еĳ���
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