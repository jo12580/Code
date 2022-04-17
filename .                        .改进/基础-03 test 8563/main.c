//---------------------------
//
//---------------------------
#include<reg52.h>
#include <PCF8563.h>
//#include <lcd1602.h>
typedef unsigned char uchar ;
typedef unsigned int  uint	;
sbit key_play=P1^6;  //语音播放引脚定义 
sbit led0=P1^4;
sbit led1=P1^5;
sbit rest=P2^7; //语音芯片复位引脚定义
sbit dat=P2^6;  //语音芯片数据线引脚定义
sbit busy=P2^5;  //语音芯片忙引脚定义
//uchar a,b=1;
void music(uchar i);
void  delay(uint i);
extern unsigned char year,mon,day,hou,min,sec,week,last_sec;

//=============================
void main()
{
	
	delay(4000);
	led0=0;
	led1=1;
	write_time_8563();		 //写日期初值,数据放于 time[7] 
	read_time_8563();		//读日期,数据放于 year,mon,day,hou,min,sec,week
	while(1)
	{	last_sec=sec;
		read_time_8563();
		if(last_sec!=sec)   //
		{
			led0=~led0;
			led1=~led1;
			music(sec/10+1);
			delay(100);
			while(!busy);
			
			music(sec%10+1);
			delay(100);
			while(!busy);
			
//			music(15);
//			delay(100);
//			while(!busy);
		}
		
	}
}

//---------------------------
//
//---------------------------
void  delay(uint i)
{
while(i--);
}

//---------------------------
//语音播放子程序
//变量i取值从1-32，分别播放32段语音素材

//---------------------------
void music(uchar i)
{	
	uchar a;
	rest=0;
	delay(50);
	rest=1;					
	delay(50);
	rest=~rest;
	for(a=0;a<i;a++)
       {
		dat=0;
		delay(50);
		dat=~dat;
		delay(50);
		dat=~dat;
	   }
}