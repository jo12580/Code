
#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <reg52.h>
sbit sda_8563 = P3^4;
sbit scl_8563 = P3^5;

//与PCF8563有关的函数声明

void read_time_8563(void);		//读日期,数据放于 year,mon,day,hou,min,sec,week
void write_time_8563(void);		 //写日期初值,数据放于 time[7]

void delay_8563(unsigned int);	//=======延迟子程序=====
void  start(void); 	  //=======开始子程序=====
void stop(void);		//=======结束子程序=====
unsigned char  writebyte_8563(unsigned char ); //=======写一个字节=====
unsigned char  readbyte_8563(void);  //======读一个字节=======
#endif