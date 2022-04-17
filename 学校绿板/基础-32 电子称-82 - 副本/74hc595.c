#include "reg52.h"
#include "intrins.h"
#include "74hc595.h"


unsigned char code led_code[] =    //
//{0xff,0xff,0x01,0x02,0x04,
//0x08,0x10,0x20,0x40,0x80};  //

{ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 
0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 
0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };//


void HC595SendData(unsigned char SendVal,unsigned char ctr)
{  
	unsigned char i,m;
	m=led_code[SendVal];

	if(ctr==2) m=m&0xdf;  //
			
	for(i=0;i<8;i++) 
	{
	   	SCK_595=0;
		if(m&0x80)   //
		DATA_595=1;                                                                                                                                                                                                               //set dataline high  0X80  最高位与SendVal左移的最高位 进行逻辑运算
		else 
		DATA_595=0;
	 	m=m<<1;   //
		SCK_595=1;  
		_nop_();	 //
		_nop_();	 //
	}  
}




void display_date(unsigned char year,month,day)
{	
	unsigned char year_h,year_l,month_h,month_l,day_h,day_l;
	
	year_h=year/10;
	year_l=year%10;
	month_h=month/10;
	month_l=month%10;
	day_h=day/10;
	day_l=day%10;
	
	HC595SendData(year_h,1);
	HC595SendData(year_l,1);
	HC595SendData(month_h,1);	
	HC595SendData(month_l,1);
	HC595SendData(day_h,1);
	HC595SendData(day_l,1);

	RCK_595=0; //set dataline low
	_nop_();  //短暂延时
	_nop_();  //短暂延时
	RCK_595=1;
}


