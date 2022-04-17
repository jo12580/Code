#include<reg52.h>
#include <PCF8563.h>
#include<voice.h>
#include "74hc595.h"
typedef unsigned char uchar ;
typedef unsigned int  uint	;

extern unsigned char year,mon,day,hou,min,sec,week,last_sec;


void main()
{
		year=0x19; //给定今天日期初值
		mon=0x12;  //
		day=0x03;  //
		hou=0x22;  //给定今天时间初值
		min=0x25;  //
		sec=0x52;  //
		week=0x02; //
	

	
	write_time_8563();		 //写日期初值,数据放于 time[7] 
	read_time_8563();		//读日期,数据放于 year,mon,day,hou,min,sec,week
	while(1)
	{	last_sec=sec;
		read_time_8563(); // year,mon,day,hou,min,sec
		if(last_sec!=sec)   //
		{
			display_time(hou,min,sec,week);
		
	}
}
}
