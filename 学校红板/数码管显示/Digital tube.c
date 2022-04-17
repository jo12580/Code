#include"Digital tube.h"

 u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

void DigDisplay()
{
	unsigned char i;
	unsigned int j;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第7位
			case(1):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(2):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(3):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(4):
				LSA=1;LSB=1;LSC=0; break;//显示第3位 
			case(5):
				LSA=0;LSB=1;LSC=0; break;//显示第2位 
			case(6):
				LSA=1;LSB=0;LSC=0; break;//显示第1位 
			case(7):
				LSA=0;LSB=0;LSC=0; break;//显示第0位	
		}
		P0=smgduan[i];//发送段码
		j=10;						 //扫描间隔时间设定
		while(j--);	
	P0=0x00;//消隐
	}
}