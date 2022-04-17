#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include"LED dot.h"
#include<intrins.h>

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;

void delay(int i)
{
	while(i--);	
}

/*******************************************************************************
* 函数名         : Hc595SendByte(u8 dat)
* 函数功能		   : 向74HC595发送一个字节的数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/


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
