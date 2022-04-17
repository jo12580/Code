#include <reg52.h>
#include "PCF8563.h"
unsigned char year,mon,day,hou,min,sec,week,last_sec;
const unsigned char  time[7]={0x09,0x11,0x20,0x05,0x11,0x14,0x30}; //���ó�ֵ����.��.��.����.ʱ.��.��

//year=0x15;
//mon=0x12;
//day=0x26;
//hou=0x23;
//min=0x59;
//sec=0x50;,
//week=0x06;
//last_week=7;

//=======�ӳ��ӳ���===== 
void delay_8563(unsigned int i)
{
    while(i--);
}

void  start(void) {  //��ʼ�ӳ���
        sda_8563 = 1;
	scl_8563 = 1;
        sda_8563 = 0;
	scl_8563 = 0;

}
//=====
void stop(void) {   //�����ӳ���
    scl_8563 = 0;
    sda_8563 = 0;
    scl_8563 = 1;
    sda_8563 = 1;
}


//=======дһ���ֽ�=====
unsigned char  writebyte_8563(unsigned char dat)
{
    unsigned int i;
	unsigned char  ack=1;
	scl_8563 = 0;
	for(i=8;i>0;i--)
	    {
		 if(dat&0x80){sda_8563 = 1;}
		 else        {sda_8563 = 0;}
		 scl_8563 = 1;
		 scl_8563 = 0;
		 dat<<=1;
		}  
	sda_8563 = 1;
	scl_8563 = 1;
      while(sda_8563){i++;if(i==1000)break;}
	ack = sda_8563;
	scl_8563 = 0;
	return(ack);  //��Ӧ���򷵻�0���޵ķ���1 
}


//======��һ���ֽ�=======
unsigned char readbyte_8563(void)
{
    unsigned char i=8;
	unsigned char dat=0x00;
	scl_8563 = 0;
	sda_8563 = 1;
	for(;i>0;i--)	    {
	   scl_8563 = 1;
	   dat=dat<<1;
	   if(sda_8563)dat|=0x01;
           scl_8563 = 0;
	}
        sda_8563 = 1;
	scl_8563 = 1;
	scl_8563 = 0;
	return(dat);
}


/*
//=====��ʼ��pcf8563=======
void Init_pcf8563(void)  {
    unsigned char i,j;
    for(i=8;i<8;i--)  {
	if(i==4)j=(time[i])&0x07;
	else    j=( ((time[i]/10)<<4 )&0xf0 )|( (time[i]%10)&0x0f );
	start();
	writebyte_8563(0xa2);	    //ѡ��д8563оƬ
	writebyte_8563(i+0x02);   //д��ַ
	writebyte_8563(j);	    //д����
 }  }


*/

//=========������==========
void read_time_8563(void)
{   
    unsigned char i;
	 P2=0x00;
	for(i=0;i<7;i++)  {
	   start();
	   writebyte_8563(0xa2);	    //ѡ��д8563оƬ
	   writebyte_8563(i+0x02);	//д��ַ
	   start();
	   writebyte_8563(0xa3);		//ѡ���8563оƬ
	   switch(i) {
		case 0:sec= ( readbyte_8563() )&0x7f;break;
		case 1:min= ( readbyte_8563() )&0x7f;break;
		case 2:hou= ( readbyte_8563() )&0x3f;break;
		case 3:day= ( readbyte_8563() )&0x3f;break;
		case 4:week=( readbyte_8563() )&0x07;break;
		case 5:mon= ( readbyte_8563() )&0x1f;break;
		case 6:year=( readbyte_8563() )&0xff;break;
		default:break;
		}
	    stop();
	   }
	sec=((sec&0xf0)>>4&0x0f)*10+(sec&0x0f);//BCD��ת����10������
	min=((min&0xf0)>>4&0x0f)*10+(min&0x0f);//BCD��ת����10������
	hou=((hou&0xf0)>>4&0x0f)*10+(hou&0x0f);//BCD��ת����10������
	day=((day&0xf0)>>4&0x0f)*10+(day&0x0f);//BCD��ת����10������
	mon=((mon&0xf0)>>4&0x0f)*10+(mon&0x0f);//BCD��ת����10������
	year=((year&0xf0)>>4&0x0f)*10+(year&0x0f);//BCD��ת����10������	

}

//=========д����============
void write_time_8563(void)  {
	start();   //д���
	writebyte_8563(0xa2);
//	 P2=0x55;
	writebyte_8563(0x08);
	writebyte_8563(year);

   	
	stop();

	start();  //д�·�
	writebyte_8563(0xa2);
	writebyte_8563(0x07);
	writebyte_8563(mon);
	stop();

	start();   //д��
	writebyte_8563(0xa2);
	writebyte_8563(0x05);
	writebyte_8563(day);
	stop();
	delay_8563(2);

	start();   //дʱ
	writebyte_8563(0xa2);
	writebyte_8563(0x04);
	writebyte_8563(hou);
	stop();
	delay_8563(2);
	 
	start();   //д��
	writebyte_8563(0xa2);
	writebyte_8563(0x03);
	writebyte_8563(min);
	stop();
	delay_8563(2);

	start();   //д��
	writebyte_8563(0xa2);
	writebyte_8563(0x02);
	writebyte_8563(sec);
	stop();
	delay_8563(2);

	start();  //д����
	writebyte_8563(0xa2);
	writebyte_8563(0x06);
	writebyte_8563(week);
	stop();
	delay_8563(2);
} 




