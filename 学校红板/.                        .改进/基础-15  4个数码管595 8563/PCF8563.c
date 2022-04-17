#include <reg52.h>
#include "PCF8563.h"
unsigned char year,mon,day,hou,min,sec,week,last_sec;
const unsigned char  time[7]={0x09,0x11,0x20,0x05,0x11,0x14,0x30}; //设置初值：年.月.日.星期.时.分.秒

//year=0x15;
//mon=0x12;
//day=0x26;
//hou=0x23;
//min=0x59;
//sec=0x50;,
//week=0x06;
//last_week=7;

//=======延迟子程序===== 
void delay_8563(unsigned int i)
{
    while(i--);
}

void  start(void) {  //开始子程序
        sda_8563 = 1;
	scl_8563 = 1;
        sda_8563 = 0;
	scl_8563 = 0;

}
//=====
void stop(void) {   //结束子程序
    scl_8563 = 0;
    sda_8563 = 0;
    scl_8563 = 1;
    sda_8563 = 1;
}


//=======写一个字节=====
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
	return(ack);  //有应答则返回0，无的返回1 
}


//======读一个字节=======
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
//=====初始化pcf8563=======
void Init_pcf8563(void)  {
    unsigned char i,j;
    for(i=8;i<8;i--)  {
	if(i==4)j=(time[i])&0x07;
	else    j=( ((time[i]/10)<<4 )&0xf0 )|( (time[i]%10)&0x0f );
	start();
	writebyte_8563(0xa2);	    //选择写8563芯片
	writebyte_8563(i+0x02);   //写地址
	writebyte_8563(j);	    //写数据
 }  }


*/

//=========读日期==========
void read_time_8563(void)
{   
    unsigned char i;
	 P2=0x00;
	for(i=0;i<7;i++)  {
	   start();
	   writebyte_8563(0xa2);	    //选择写8563芯片
	   writebyte_8563(i+0x02);	//写地址
	   start();
	   writebyte_8563(0xa3);		//选择读8563芯片
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
	sec=((sec&0xf0)>>4&0x0f)*10+(sec&0x0f);//BCD码转换成10进制数
	min=((min&0xf0)>>4&0x0f)*10+(min&0x0f);//BCD码转换成10进制数
	hou=((hou&0xf0)>>4&0x0f)*10+(hou&0x0f);//BCD码转换成10进制数
	day=((day&0xf0)>>4&0x0f)*10+(day&0x0f);//BCD码转换成10进制数
	mon=((mon&0xf0)>>4&0x0f)*10+(mon&0x0f);//BCD码转换成10进制数
	year=((year&0xf0)>>4&0x0f)*10+(year&0x0f);//BCD码转换成10进制数	

}

//=========写日期============
void write_time_8563(void)  {
	start();   //写年份
	writebyte_8563(0xa2);
//	 P2=0x55;
	writebyte_8563(0x08);
	writebyte_8563(year);

   	
	stop();

	start();  //写月份
	writebyte_8563(0xa2);
	writebyte_8563(0x07);
	writebyte_8563(mon);
	stop();

	start();   //写日
	writebyte_8563(0xa2);
	writebyte_8563(0x05);
	writebyte_8563(day);
	stop();
	delay_8563(2);

	start();   //写时
	writebyte_8563(0xa2);
	writebyte_8563(0x04);
	writebyte_8563(hou);
	stop();
	delay_8563(2);
	 
	start();   //写分
	writebyte_8563(0xa2);
	writebyte_8563(0x03);
	writebyte_8563(min);
	stop();
	delay_8563(2);

	start();   //写秒
	writebyte_8563(0xa2);
	writebyte_8563(0x02);
	writebyte_8563(sec);
	stop();
	delay_8563(2);

	start();  //写星期
	writebyte_8563(0xa2);
	writebyte_8563(0x06);
	writebyte_8563(week);
	stop();
	delay_8563(2);
} 




