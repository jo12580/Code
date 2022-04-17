#include<reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

sbit ENA = P2^0;   //右点机使能
sbit IN1 = P2^1;   //为0右轮反转
sbit IN2 = P2^2;   //为0右轮正转
sbit IN3 = P2^3;   //为0左轮正转
sbit IN4 = P2^4;   //为0左轮反转
sbit ENB = P2^5;   //左电机使能

sbit left1 = P1^3;
sbit left2 = P1^2;
sbit right1 = P1^1;
sbit right2 = P1^0;

u8 PWMCnt1 = 0;
u8 PWMCnt2 = 0;
u8 cntPWM1 = 0;
u8 cntPWM2 = 0;

void TurnRight1();
void TurnRight2()	;
void TurnLeft1();
void TurnLeft2();
void Forward()	;
void Backward();
void Stop()	;

void Timer0Init();
void Timer1Init();
void XunJi();
void delay(u16 i);

unsigned int j;

void main()
{
	Timer0Init();
	Timer1Init();
	while(1)
	{
		XunJi();
	}	
}
	
//i = 1时， 大概延时10us
void delay(u16 i)
{
    while (i--);
}

void Timer0Init()
{
	TH0 = 0xFF;
	TL0 = 0xA3;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void Timer1Init(void)		//100微秒@11.0592MHz
{
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	EA = 1;
	ET0 = 1;
}

void TurnRight1()		   //右转
{
//	IN1 = 0;	//右轮反转
//	IN2 = 1;

	IN3 = 1;	//左轮正转
	IN4 = 0;
	cntPWM1 = 30;
	cntPWM2	= 15;
}

void TurnRight2()		   //右转
{
//	IN1 = 0;	//右轮反转
//	IN2 = 1;

	IN3 = 1;	//左轮正转
	IN4 = 0;
	cntPWM1 = 25;
	cntPWM2	= 15;
}

void TurnLeft1()				//左转
{
	IN1 = 1;
	IN2 = 0;   	//右轮正转

//	IN3 = 0;
//	IN4 = 1;   	//左轮反转
	cntPWM1 = 15;
	cntPWM2	= 30;
}			  	

void TurnLeft2()				//左转
{
	IN1 = 1;
	IN2 = 0;   	//右轮正转

//	IN3 = 0;
//	IN4 = 1;   	//左轮反转
	cntPWM1 = 15;
	cntPWM2	= 25;
}	
	
void Forward()				 //前进
{
	IN1 = 1;
	IN2 = 0;   	//右轮正转

	IN3 = 1;   	//左轮正转
	IN4 = 0;
	
	cntPWM1 = 30;
	cntPWM2	= 30;
}

void Backward()					 //后退
{
	IN1 = 0;	//右轮反转
	IN2 = 1;

	IN3 = 0;
	IN4 = 1;	//左轮反转
	cntPWM1 = 20;
	cntPWM2	= 20;
}

void Stop()					  //停止
{
	IN1 = 0;
	IN2 = 0;

	IN3 = 0;
	IN4 = 0;
}
void XunJi()

{
	unsigned char flag = 0;

	if((left1 == 0)&&(left2 == 0)&&(right1 == 0)&&(right2 == 0))   //0 0 0 0
		flag = 0;

	if((left1 == 0)&&(left2 == 0)&&(right1 == 0)&&(right2 == 1))   //0 0 0 1
		flag = 1;

	if((left1 == 0)&&(left2 == 0)&&(right1 == 1)&&(right2 == 0))   //0 0 1 0
		flag = 0;

	if((left1 == 0)&&(left2 == 0)&&(right1 == 1)&&(right2 == 1))   //0 0 1 1
		flag = 1;

	if((left1 == 0)&&(left2 == 1)&&(right1 == 0)&&(right2 == 0))   //0 1 0 0
		flag = 0;

	if((left1 == 0)&&(left2 == 1)&&(right1 == 0)&&(right2 == 1))   //0 1 0 1
		flag = 4;

	if((left1 == 0)&&(left2 == 1)&&(right1 == 1)&&(right2 == 0))   //0 1 1 0
		flag = 0;
	
	if((left1 == 0)&&(left2 == 1)&&(right1 == 1)&&(right2 == 1))    //0 1 1 1
		flag = 1;
	
	if((left1 == 1)&&(left2 == 0)&&(right1 == 0)&&(right2 == 0))   //1 0 0 0
		flag = 3;
	
	if((left1 == 1)&&(left2 == 0)&&(right1 == 0)&&(right2 == 1))   //1 0 0 1
		flag = 0;
	
	if((left1 == 1)&&(left2 == 0)&&(right1 == 1)&&(right2 == 0))   //1 0 1 0
		flag = 2;
	
	if((left1 == 1)&&(left2 == 0)&&(right1 == 1)&&(right2 == 1))   //1 0 1 1
		flag = 0;
	
	if((left1 == 1)&&(left2 == 1)&&(right1 == 0)&&(right2 == 0))   //1 1 0 0
		flag = 3;
	 
	if((left1 == 1)&&(left2 == 1)&&(right1 == 0)&&(right2 == 1))   //1 1 0 1
		flag = 0;

	if((left1 == 1)&&(left2 == 1)&&(right1 == 1)&&(right2 == 0))   //1 1 1 0
		flag = 3;
	
	if((left1 == 1)&&(left2 == 1)&&(right1 == 1)&&(right2 == 1))   //1 1 1 1
		flag = 5;
	
	switch(flag)
	{
		case 0:Forward();break;
		
		case 1:TurnLeft1();break;	
		
		case 2:TurnLeft2();break;
	
		case 3:TurnRight1();break;
		
		case 4:TurnRight2();break;
		
		default:Stop();break;
	}
}

void InterruptTime0() interrupt 1
{
	PWMCnt1++;
	PWMCnt2++;
	
	if(PWMCnt1  >= 200)
	{
		PWMCnt1 = 0;
	}
	if(PWMCnt1 <= cntPWM1)
	{
		ENA = 1;
	}
	else
	{
		ENA = 0;
	}

	if(PWMCnt2 >= 200)
	{
		PWMCnt2 = 0;
	}
	if(PWMCnt2 <= cntPWM2)
	{
		ENB = 1;
	}
	else
	{
		ENB = 0;
	}

	TH0 = (65536 - 50)/256;
	TL0 = (65536 - 50)%256;
}

void InterruptTime1() interrupt 3
{
	unsigned int i;
	i++;
	if(i==1000)
		if(i==5)
		{
			i=0;
			j++;
		}
}