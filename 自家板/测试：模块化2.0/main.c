#include <reg52.h>
#include "key.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include"ds1302.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

#define GPIO_DIG P0

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

char num=0;
u8 DisplayData[8];
u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值


/*******************************************************************************
* 函 数 名         : datapros()
* 函数功能		   : 时间读取处理转换函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void datapros() 	 
{
   	Ds1302ReadTime();
	DisplayData[0] = smgduan[TIME[2]/16];				//时
	DisplayData[1] = smgduan[TIME[2]&0x0f];				 
	DisplayData[2] = 0x40;
	DisplayData[3] = smgduan[TIME[1]/16];				//分
	DisplayData[4] = smgduan[TIME[1]&0x0f];	
	DisplayData[5] = 0x40;
	DisplayData[6] = smgduan[TIME[0]/16];				//秒
	DisplayData[7] = smgduan[TIME[0]&0x0f];
}


/*******************************************************************************
* 函数名         :DigDisplay()
* 函数功能		 :数码管显示函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<7;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第0位
			case(1):
				LSA=0;LSB=1;LSC=1; break;//显示第1位
			case(2):
				LSA=1;LSB=0;LSC=1; break;//显示第2位
			case(3):
				LSA=0;LSB=0;LSC=1; break;//显示第3位
			case(4):
				LSA=1;LSB=1;LSC=0; break;//显示第4位
			case(5):
				LSA=0;LSB=1;LSC=0; break;//显示第5位
			case(6):
				LSA=1;LSB=0;LSC=0; break;//显示第6位
		}
		P0=DisplayData[i];//发送数据
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}		
}




void main()
{	
	LSA=1; //给一个数码管提供位选
	LSB=1;
	LSC=1;

	while(1)
	{
		u8 k;
		k=KeyDown();			   
		GPIO_DIG=smgduan[k];	  
	}		
}

