
#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <reg52.h>
sbit sda_8563 = P1^2;
sbit scl_8563 = P1^3;


//unsigned char year=0x15,mon=0x12,day=0x26,hou=0x23,min=0x59,sec=0x50,week=0x06,last_week=7;
//const unsigned char  time[7]={0x09,0x11,0x20,0x05,0x11,0x14,0x30}; //���ó�ֵ����.��.��.����.ʱ.��.��
//��PCF8563�йصĺ�������
void delay_8563(unsigned int);	//=======�ӳ��ӳ���=====
void  start(void); 	  //=======��ʼ�ӳ���=====
void stop(void);		//=======�����ӳ���=====
unsigned char  writebyte_8563(unsigned char ); //=======дһ���ֽ�=====
unsigned char  readbyte_8563(void);  //======��һ���ֽ�=======
void read_time_8563(void);		//������,���ݷ��� year,mon,day,hou,min,sec,week
void write_time_8563(void);		 //д���ڳ�ֵ,���ݷ��� time[7]

#endif