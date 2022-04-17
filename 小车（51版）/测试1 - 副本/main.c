#include<reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

sbit ENA = P2^0;   //�ҵ��ʹ��
sbit IN1 = P2^1;   //Ϊ0���ַ�ת
sbit IN2 = P2^2;   //Ϊ0������ת
sbit IN3 = P2^3;   //Ϊ0������ת
sbit IN4 = P2^4;   //Ϊ0���ַ�ת
sbit ENB = P2^5;   //����ʹ��

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
	
//i = 1ʱ�� �����ʱ10us
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

void Timer1Init(void)		//100΢��@11.0592MHz
{
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xD4;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	EA = 1;
	ET0 = 1;
}

void TurnRight1()		   //��ת
{
//	IN1 = 0;	//���ַ�ת
//	IN2 = 1;

	IN3 = 1;	//������ת
	IN4 = 0;
	cntPWM1 = 30;
	cntPWM2	= 15;
}

void TurnRight2()		   //��ת
{
//	IN1 = 0;	//���ַ�ת
//	IN2 = 1;

	IN3 = 1;	//������ת
	IN4 = 0;
	cntPWM1 = 25;
	cntPWM2	= 15;
}

void TurnLeft1()				//��ת
{
	IN1 = 1;
	IN2 = 0;   	//������ת

//	IN3 = 0;
//	IN4 = 1;   	//���ַ�ת
	cntPWM1 = 15;
	cntPWM2	= 30;
}			  	

void TurnLeft2()				//��ת
{
	IN1 = 1;
	IN2 = 0;   	//������ת

//	IN3 = 0;
//	IN4 = 1;   	//���ַ�ת
	cntPWM1 = 15;
	cntPWM2	= 25;
}	
	
void Forward()				 //ǰ��
{
	IN1 = 1;
	IN2 = 0;   	//������ת

	IN3 = 1;   	//������ת
	IN4 = 0;
	
	cntPWM1 = 30;
	cntPWM2	= 30;
}

void Backward()					 //����
{
	IN1 = 0;	//���ַ�ת
	IN2 = 1;

	IN3 = 0;
	IN4 = 1;	//���ַ�ת
	cntPWM1 = 20;
	cntPWM2	= 20;
}

void Stop()					  //ֹͣ
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