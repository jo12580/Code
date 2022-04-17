#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include"Digital tube.h"

void main()
{	
	while(1)
	{	
		DigDisplay();  //数码管显示函数	
	}		
}