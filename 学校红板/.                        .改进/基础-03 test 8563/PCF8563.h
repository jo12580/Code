
#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <reg52.h>
sbit sda_8563 = P1^2;
sbit scl_8563 = P1^3;


//unsigned char year=0x15,mon=0x12,day=0x26,hou=0x23,min=0x59,sec=0x50,week=0x06,last_week=7;
//const unsigned char  time[7]={0x09,0x11,0x20,0x05,0x11,0x14,0x30}; //设置初值：年.月.日.星期.时.分.秒
//与PCF8563有关的函数声明
void delay_8563(unsigned int);	//=======延迟子程序=====
void  start(void); 	  //=======开始子程序=====
void stop(void);		//=======结束子程序=====
unsigned char  writebyte_8563(unsigned char ); //=======写一个字节=====
unsigned char  readbyte_8563(void);  //======读一个字节=======
void read_time_8563(void);		//读日期,数据放于 year,mon,day,hou,min,sec,week
void write_time_8563(void);		 //写日期初值,数据放于 time[7]

#endif