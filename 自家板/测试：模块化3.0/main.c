#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "ds1302.h"	
#include "key.h"



void main()
{	

	Ds1302Init();	   //第一次初始化后就可以注释该条语句，这样下次重启就不会再次初始化了
			        key();	
	while(1)
	{
		datapros();	 //数据处理函数
		DigDisplay();//数码管显示函数

	}		
}






