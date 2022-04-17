#include<reg52.h>
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
sbit k1=P1^6;  //语音播放引脚定义 
sbit k2=P1^7;  //语音播放引脚定义 
sbit Led1=P1^4;
sbit Led2=P1^5;

sbit rest=P2^7; //语音芯片复位引脚定义
sbit  dat=P2^6;  //语音芯片数据线引脚定义
sbit busy=P2^5;  //语音芯片---引脚定义
uchar a,b=1;
void music(uchar i);
void  delay(uint i);

void main()
	{
	uchar i=1;
	    Led1=0;
		Led2=1;
	    P0=0;
	
	delay(100);	
	while (1)
		{
		   	delay(5000); 
			music(34); 
			while (!busy);
			delay(5000); 	
			Led1=~Led1;
			delay(5000); 
			Led2=~Led2;	
		}
	}
 



void  delay(uint i)
	{
	while(i--);
	}
                                        

                                                              //此中间可以加语音播报内容     



void music(uchar i)                             //定义music（语音）函数
	{	
	dat=0;  
	rest=0;
	delay(50);
	rest=1;					
	delay(50);
	rest=0;
    delay(50);
	
	dat=0;
	delay(50);
	
	for(a=0;a<i;a++)
       {
		dat=1;
		delay(50);
		dat=0;
		delay(50);   
	   }
	}