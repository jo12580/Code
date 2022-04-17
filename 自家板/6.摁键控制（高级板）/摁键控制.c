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
u8 code sz0_7[8]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};//显示0~7的值
u8 code	xh031[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x4f,0x06};//显示00000031
								    
sbit k1=P3^1;                       //摁键
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;

						void LED1()
{
	u8 i;
		 while (1)
	{for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
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
		P0=sz0_7[i];//发送段码
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}
}
}
						void LED2()
{
	u8 i;
		 while (1)
		 {
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
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
}


                                  void delay(u16 i)
{
	while(i--);	
}

void abc()
		{	   while(1)
		{
		beep=~beep;
		delay(1000); //延时大约1ms   通过修改此延时时间达到不同的发声效果	
		}
		}

 void main()
{
			if(k1==0)
	{	
	if(k1==0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消除抖动 一般大约10ms
		if(k1==0)	 //再次判断按键是否按下
		{
		LED1();													    //数码管显示函数	
		}
		while(!k1);	 //检测按键是否松开
	}                                                   
	}

					
			if(k2==0)
	{		  
		delay(1000);   //消除抖动 一般大约10ms
		if(k2==0)	 //再次判断按键是否按下
		{
			   abc();
		}
		while(!k2);	 //检测按键是否松开
	
	}

			if(k3==0)
		{
				delay(1000);   //消除抖动 一般大约10ms
		if(k3==0)	 //再次判断按键是否按下
		{
	beep=1;
		}
		while(!k3);	 //检测按键是否松开
		} 
		



			if(k4==0)
	{	
         delay(1000);   //消除抖动 一般大约10ms
		if(k4==0)	 //再次判断按键是否按下
		{
      LED2();
		}
		while(!k4);	 //检测按键是否松开
	}

}





