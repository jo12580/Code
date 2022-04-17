#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "intrins.h"
#include<stdlib.h>

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 code smgduan[19]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 
0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码

void delay(u16 i)
{
	while(i--);	
}

void Delay500us()		//@12.000MHz
{
	unsigned char i, j;

	i = 6;
	j = 211;
	do
	{
		while (--j);
	} while (--i);
}



//void test(u16 a,u16 b)
//{
////	P2=0xc0;
//	P2=0xe0;
//	P0=smgduan[b];
//}

//void Display()
//{
//	u16 i,j,z;
////	for(i=0;i<=8;i++)
////	{
////			for(j=0;j<=8;j++)
////			{
////				test(i,j);
////				delay(6000);
////			}
////	}
//	for(z=0;z<16;z++)
//	{
//		P2=0xc0;
//		P0= 0xff;
//		P2=0xe0;
//		P0= smgduan [z];
//		delay (60000);
//		delay(60000);
//				delay(60000);
//				delay(60000);
//	}
//}
void Display()
{
	u16 i;
	for(i=0;i<=7;i++)
	{
		P2=0xc0;
    switch(i)
		{
			case 0:P0=0x01;break;
			case 1:P0=0x02;break;
			case 2:P0=0x04;break;
			case 3:P0=0x08;break;
			case 4:P0=0x10;break;
			case 5:P0=0x20;break;
			case 6:P0=0x40;break;
			case 7:P0=0x80;break;
		}
		P2=0xe0;
		P0= smgduan[i];
		Delay500us();	
		P0=0xff;	
	}
}

void main()
{	
	while(1)
	{	
		Display();

	}		
}



