#include "reg52.h"
#include "intrins.h"
#include "74hc595.h"

unsigned char year_2_h,year_2_l,
			  month_2_h,month_2_l,
			  day_2_h,day_2_l,
              hour_2_h,hour_2_l,
			  min_2_h,min_2_l,
			  sec_2_h,sec_2_l,
			  week_2_h,week_2_l;
int temp_int_2_h,temp_int_2_l,temp_float2int_2_h,temp_float2int_2_l;

unsigned char code led_code[] =
//{0xff,0xff,0x01,0x02,0x04,
//0x08,0x10,0x20,0x40,0x80};  //此为测试代码

{ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 
0x82, 0xF8, 0x80, 0x90, 0x88, 0x83,
0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };


//{ 0x82, 0xAF, 0xC4, 0x85, 0xA9, 0x91, 
//0x90, 0xA7, 0x80, 0x81, 0xA0, 0x98, 
//0xD2, 0x8C, 0xD0, 0xF0, 0xFD, 0x7F };


void HC595SendData(unsigned char SendVal,unsigned char ctr)
{  
	unsigned char i,m;
	m=led_code[SendVal];

	if(ctr==2) m=m&0xbf;//加个小数点  //1010 1101
			
	for(i=0;i<8;i++) 
	{
	   	SCK_595=0;
		if(m&0x80) 
		DATA_595=1;                                                                                                                                                                                                               //set dataline high  0X80  最高位与SendVal左移的最高位 进行逻辑运算
		else 
		DATA_595=0;
	 	m=m<<1;
		SCK_595=1;  
		_nop_();	 //短暂延时产生一定宽度的脉冲信号
		_nop_();	 //短暂延时	
	}
	/*以下四条语句不能放在此函数里，不然会有闪烁感
	RCK_595=0; //set dataline low
	_nop_();  //短暂延时
	_nop_();  //短暂延时
	RCK_595=1;
	应放在显示函数的后面（每传输完一组数据再拉高）*/ 	    
}

void display_time(unsigned char hour_2,min_2,sec_2,week_2)
{
	hour_2_h=hour_2/10;
	hour_2_l=hour_2%10;
	min_2_h=min_2/10;
	min_2_l=min_2%10;
	sec_2_h=sec_2/10;
	sec_2_l=sec_2%10;
	week_2_h=week_2/10;
	week_2_l=week_2%10;
	
	HC595SendData(sec_2_l,1);
	HC595SendData(sec_2_h,1);
	HC595SendData(min_2_l,1);
	HC595SendData(min_2_h,1);
	HC595SendData(hour_2_l,1);
	HC595SendData(hour_2_h,1);
	HC595SendData(10,1);
	HC595SendData(week_2_l,1);
	
	RCK_595=0; //set dataline low
	_nop_();  //短暂延时
	_nop_();  //短暂延时
	RCK_595=1;

}

void display_date(unsigned char year_2,month_2,day_2)
{
	year_2_h=year_2/10;
	year_2_l=year_2%10;
	month_2_h=month_2/10;
	month_2_l=month_2%10;
	day_2_h=day_2/10;
	day_2_l=day_2%10;

	HC595SendData(day_2_l,1);
	HC595SendData(day_2_h,1);
	HC595SendData(month_2_l,1);
	HC595SendData(month_2_h,1);
	HC595SendData(year_2_l,1);
	HC595SendData(year_2_h,1);
	HC595SendData(0,1);
	HC595SendData(2,1);

	RCK_595=0; //set dataline low
	_nop_();  //短暂延时
	_nop_();  //短暂延时
	RCK_595=1;
}

void display_temperature(int temp_int_2,temp_float2int_2)
{
	temp_int_2_h=temp_int_2/10;
	temp_int_2_l=temp_int_2%10;
	temp_float2int_2_h=temp_float2int_2/10;
	temp_float2int_2_l=temp_float2int_2%10;

	HC595SendData(temp_float2int_2_l,1);
	HC595SendData(temp_float2int_2_h,1);
	HC595SendData(temp_int_2_l,2);//加个小数点
	HC595SendData(temp_int_2_h,1);
	HC595SendData(10,1);
	HC595SendData(10,1);
	HC595SendData(10,1);
	HC595SendData(10,1);

	RCK_595=0; //set dataline low
	_nop_();  //短暂延时
	_nop_();  //短暂延时
	RCK_595=1;
}



