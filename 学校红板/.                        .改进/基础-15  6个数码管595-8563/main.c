//---------------------------
//
//---------------------------
#include<reg52.h>
#include <PCF8563.h>
#include<voice.h>
#include "74hc595.h"
typedef unsigned char uchar ;
typedef unsigned int  uint	;
sbit key_play=P1^6;  //�����������Ŷ��� 
sbit led0=P1^4;
sbit led1=P1^5;

extern unsigned char year,mon,day,hou,min,sec,week,last_sec;

//=============================
void main()
{
		year=0x19; //�����������ڳ�ֵ
		mon=0x12;  //
		day=0x03;  //
		hou=0x22;  //��������ʱ���ֵ
		min=0x25;  //
		sec=0x52;  //
		week=0x02; //
	
	led0=0;
	led1=1;
	
	write_time_8563();		 //д���ڳ�ֵ,���ݷ��� time[7] 
	read_time_8563();		//������,���ݷ��� year,mon,day,hou,min,sec,week
	while(1)
	{	last_sec=sec;
		read_time_8563(); // year,mon,day,hou,min,sec
		if(last_sec!=sec)   //
		{
			led0=~led0;
			led1=~led1;
			display_time(hou,min,sec,week);
//====================================
			voice(hou/10+2);
      if(hou>10){		voice(12); } //ʮ
			voice(hou%10+2);
			voice(14);  //						

//=====================================			
			voice(min/10+2);
      if(min>10){		voice(12); } //ʮ
			voice(min%10+2);
			voice(15);  //			
//===============================			
			voice(sec/10+2);
      if(sec>10){		voice(12); } //ʮ
			voice(sec%10+2);
			voice(16);  //��

			
		}
		
	}
}

//---------------------------
