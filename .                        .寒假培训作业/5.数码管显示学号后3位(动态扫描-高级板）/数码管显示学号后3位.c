#include <reg52.h>			
typedef unsigned int u16;	  
typedef unsigned char u8;
sbit beep=P2^5;

typedef unsigned char uchar	  ;	    //延迟定义
typedef unsigned int  uint	;
void  delay(uint i);
								  
sbit LSA=P2^2;					    //数码管
sbit LSB=P2^3;
sbit LSC=P2^4;
u8 code	xh031[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x4f,0x3f};//显示00000030

	void LED1()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管
		{
		    case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第0位
			case(1):
				LSA=0;LSB=1;LSC=1; break;//1
			case(2):
				LSA=1;LSB=0;LSC=1; break;//2
			case(3):	
				LSA=0;LSB=0;LSC=1; break;//3
			case(4):
				LSA=1;LSB=1;LSC=0; break;//4
			case(5):
				LSA=0;LSB=1;LSC=0; break;//5
			case(6):
				LSA=1;LSB=0;LSC=0; break;//6
			case(7):
				LSA=0;LSB=0;LSC=0; break;//7	
		}
		P0=xh031[i];//发送段码
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}
}
                   void delay(u16 i)
{
	while(i--);	
}



void main()
{
				while(1)
	{	
                LED1();  //数码管显示函数	
	}
}