#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "lcd.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 Disp[]="我爱电子，我爱师大";
u8 aaa[]="欧阳泽军 201912700030" ;

void main(void)
{

   if(k2==0)
{	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp[i]);	
	}
	while(1);	
}	
	
	
	if(k1==0)	
{
	   	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
		{
		LcdWriteData(aaa[i]);	
		}
	while(1);
}
		
}
