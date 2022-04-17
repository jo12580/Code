	#include<reg52.h>
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
sbit k1=P1^6;  //语音播放引脚定义 
sbit k2=P1^7;  //语音播放引脚定义 
sbit k3=P1^0;  //语音播放引脚定义 
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
			
	    Led1=0;
		Led2=1;
	    P0=0;
	
	delay(100);	
	while (1)  {

	         if(k1==0)
{music(26);
while(!busy);		 //音乐
music(36) ;	  //欢迎再次光临
while(!busy) ;
music(37)  ;}  //广西师范大学


             if(k2==0)
{music(26);
while(!busy);		 //音乐
music(36);	  //欢迎再次光临
  while(!busy) ;
music(38)  ;  //电子工程学院
  while(!busy)	;
music(46) ;	}	//大学生创新基地


             if(k3==0)
{music(26);
while(!busy);		 //音乐
music(28);		 //今天是
  while(!busy) ;
music(4);		 //2
  while(!busy) ;
music(2);		 //0
  while(!busy);
music(3);		 //	1
while(!busy)  ;
music(11);		 //	9
while(!busy);
music(17);		 //年
while(!busy);
music(12);		 //十
while(!busy) ;
music(3);		 //一
while(!busy) ;
music(18);		 //月
while(!busy) ;
music(4);		 //二
while(!busy) ;
music(12);		 //十 
while(!busy) ;
music(19);		 //日
while(!busy) ;
music(34);		 //欢迎光临本店
while(!busy) ;
music(35);	}	 //欢迎使用本公司产品

   }    
}
 

void  delay(uint i)
{
	while(i--);
}

void music(uchar i)								//语音函数的声明
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
		