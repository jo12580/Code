
#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <reg52.h>
sbit sda_8563 = P3^4;
sbit scl_8563 = P3^5;

//��PCF8563�йصĺ�������

void read_time_8563(void);		//������,���ݷ��� year,mon,day,hou,min,sec,week
void write_time_8563(void);		 //д���ڳ�ֵ,���ݷ��� time[7]

void delay_8563(unsigned int);	//=======�ӳ��ӳ���=====
void  start(void); 	  //=======��ʼ�ӳ���=====
void stop(void);		//=======�����ӳ���=====
unsigned char  writebyte_8563(unsigned char ); //=======дһ���ֽ�=====
unsigned char  readbyte_8563(void);  //======��һ���ֽ�=======
#endif