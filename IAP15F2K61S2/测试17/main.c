/*******************************************************************************  
* �ļ����ƣ����ű���Ƭ������������Ƶ--��������ʱ��оƬDS1302
* ʵ��Ŀ�ģ����ű���Ƭ������������ģ�飺ʱ��оƬDS1302
* ����˵���������˽��йر�����������Ƶ���⣬�������������\
*					 ��ӭ�����Ա�������ַ��shop117015787.taobao.com
* ��    �ߣ���Ƶ��ӹ�����
* ���ڰ汾��2019-1/V1.0
*******************************************************************************/
#include<stc15f2k60s2.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
sbit SCK=P1^7;
sbit SDA=P2^3;
sbit RST=P1^3;

uchar tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar yi,er,san,si,wu,liu,qi,ba;
bit S4,S5,S6,S7;
uchar num;

uchar Init_Time[]={50,59,23,16,10,2,19};
uchar shijian[7];

void Allinit(void);
void Delayms(int ms);
void Display1(uchar yi,uchar er);
void Display2(uchar san,uchar si);
void Display3(uchar wu,uchar liu);
void Display4(uchar qi,uchar ba);

void DS1302_Init(void);
void DS1302_Get(void);

void main(void)
{
	Allinit();
	DS1302_Init();
	yi=0;er=8;san=10;si=5;wu=9;liu=10;qi=5;ba=0;
	while(1)
	{
		DS1302_Get();
		yi=shijian[2]/10;er=shijian[2]%10;san=10;
		si=shijian[1]/10;wu=shijian[1]%10;liu=10;
		qi=shijian[0]/10;ba=shijian[0]%10;
		Display1(yi,er);
		Display2(san,si);
		Display3(wu,liu);
		Display4(qi,ba);
	}
}

void Write_Byte(unsigned temp)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SCK=0;
		SDA=temp&0x01;
		temp>>=1;
		SCK=1;
	}
}

void Write_DS1302(unsigned char address,unsigned char dat)
{
	RST=0;
	_nop_();
	SCK=0;
	_nop_();
	RST=1;	
	Write_Byte(address);
	Write_Byte((dat/10<<4)|(dat%10));	
}

unsigned char Read_DS1302(unsigned char address)
{
	unsigned char i,temp=0x00,dat1,dat2;
	RST=0;
	_nop_();
	SCK=0;
	_nop_();
	RST=1;	
	Write_Byte(address);
	
	for(i=0;i<8;i++)
	{
		SCK=0;
		temp>>=1;
		if(SDA)temp|=0x80;
		SCK=1;
	}
	
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	dat1=temp/16;
	dat2=temp%16;
	temp=10*dat1+dat2;
	return temp;
}

void DS1302_Init()
{
	unsigned char i,add;
	add=0x80;
	Write_DS1302(0x8e,0x00);
	for(i=0;i<=7;i++)
	{
		Write_DS1302(add,Init_Time[i]);
		add=add+2;
	}
	Write_DS1302(0x8e,0xff);
}

void DS1302_Get()
{
	unsigned char i,add;
	add=0x81;
	Write_DS1302(0x8e,0x00);
	for(i=0;i<=7;i++)
	{
		shijian[i]=Read_DS1302(add);
		add=add+2;
	}
	Write_DS1302(0x8e,0xff);
}

void Display1(uchar yi,uchar er)
{
	P2=0XC0;
	P0=0X01;
	P2=0XE0;
	P0=tab[yi];
	Delayms(1);
	
	P2=0XC0;
	P0=0X02;
	P2=0XE0;
	P0=tab[er];
	Delayms(1);
}

void Display2(uchar san,uchar si)
{
	P2=0XC0;
	P0=0X04;
	P2=0XE0;
	P0=tab[san];
	Delayms(1);
	
	P2=0XC0;
	P0=0X08;
	P2=0XE0;
	P0=tab[si];
	Delayms(1);
}

void Display3(uchar wu,uchar liu)
{
	P2=0XC0;
	P0=0X10;
	P2=0XE0;
	P0=tab[wu];
	Delayms(1);
	
	P2=0XC0;
	P0=0X20;
	P2=0XE0;
	P0=tab[liu];
	Delayms(1);
}

void Display4(uchar qi,uchar ba)
{
	P2=0XC0;
	P0=0X40;
	P2=0XE0;
	P0=tab[qi];
	Delayms(1);
	
	P2=0XC0;
	P0=0X80;
	P2=0XE0;
	P0=tab[ba];
	Delayms(1);
}

void Allinit(void)
{
	P2=0XA0;//�򿪿��Ʒ�������573
	P0=0X00;//�رշ������̵���
	
	P2=0XC0;//�򿪿��������λѡ��573
	P0=0XFF;//ѡ�����������
	P2=0XE0;//�򿪿�������ܶ�ѡ��573
	P0=0XFF;//�ر����е������
	
	P2=0X80;//�򿪿���LED��573
	P0=0XFF;//�ر����е�LED
}

void Delayms(int ms)
{
	int i,j;
	for(i=0;i<ms;i++)
		for(j=845;j>0;j--);
}

/*
void Delay1ms(int ms)		//@11.0592MHz
{
	unsigned char i, j,k;

	for(k=0;k<ms;k++)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}
*/