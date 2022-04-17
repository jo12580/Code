#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include<intrins.h>

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;
sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;
u8 ledduan[]={0x30,0x48,0x44,0x22,0x22,0x44,0x48,0x30};            //D	列	
u8 ou[]={0xea,0xae,0xea,0x00,0x82,0x61,0x8f,0x00};
u8 xt[]={0xa0,0x40,0xa8,0x0e,0x88,0xe0,0x80,0x00};
u8 xl[]={0x00,0x30,0x34,0x02,0x02,0x34,0x30,0x00}  ;
				   
u8 ledwei[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};			   //P	横	 (从底往上输入高电压)
/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* 函数名         : Hc595SendByte(u8 dat)
* 函数功能		   : 向74HC595发送一个字节的数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Hc595SendByte(u8 dat)
{											   
	u8 a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();					   //延迟
	_nop_();
	RCLK=0;
}

void main()
  {      	while (1)  {
				if(k2==0)//心							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //位选
			Hc595SendByte(ledduan[i]);	//发送段选数据
			delay(100);		   //延时
			Hc595SendByte(0x00);  //消隐
		}	
	}		
}
  






  			if(k1==0)	//oyzj						 
{    u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //位选
			Hc595SendByte(ou[i]);	//发送段选数据
			delay(100);		   //延时
			Hc595SendByte(0x00);  //消隐
		}	
	}		
}
 





	if(k3==0)							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //位选
			Hc595SendByte(xt[i]);	//发送段选数据
			delay(100);		   //延时
			Hc595SendByte(0x00);  //消隐
		}	
	}		

  }




if(k4==0)							 
{	u8 i;
	while(1)
	{
		P0=0x7f;
		for(i=0;i<8;i++)
		{
			P0=ledwei[i];		  //位选
			Hc595SendByte(xl[i]);	//发送段选数据
			delay(100);		   //延时
			Hc595SendByte(0x00);  //消隐
		}	
	}		
}
}
}
