#include<reg52.h>
#include <IIC-1602.h>
//#include"infrared.h"
//#include"delay.h"

void  delay(unsigned int i);
sbit LED1=P3^4;  //主要做测试程序用
sbit IRIN=P3^2;

unsigned char IrValue[6];
unsigned char  Time;
code unsigned cmd_infrareed[]={0x19,0x45,0x46,0x47,0x44,0x40,0x43,0x07,0x15,0x09,0x16,0x0b,0x18,0x08,0x1c,0x5a,0x52}; // 0~9   A~G

void IrInit()  // 外部中断0初始化
{
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA=1;	//打开总中断
	IRIN=1;//初始化端口
}

void main()
{  
	IrInit();  // 外部中断0初始化
	Lcd1602Iinitial();
	DispOneChar(0,0,'1');        //显示一个字符
  disp_string(0,0,"123456");       //显示一个字符串
	delay(40000);
  disp_string(0,0,"      ");       //显示多个看不见的空格，相当于清屏
while(1)
{
	   	
		switch(IrValue[2])
	 		 {
				 case(0x19): disp_string(0,1," 0");  //按键0
							  break;					  
		     case(0x45): disp_string(0,1," 1");  //按键1
							  break;					 
				 case(0x46): disp_string(0,1," 2");  //按键2 
							  break;					  
				 case(0x47): disp_string(0,1," 3");  //按键3
							  break;					  
				 case(0x44):  disp_string(0,1," 4");  //按键4
							  break;					 
				 case(0x40):  disp_string(0,1," 5");  //按键5
							  break;					  
				 case(0x43):  disp_string(0,1," 6");  //按键6
							  break;					  
				 case(0x07):  disp_string(0,1," 7");  //按键7
							  break;					  
				 case(0x15):  disp_string(0,1," 8");  //按键8
							  break;					  
				 case(0x09):  disp_string(0,1," 9");  //按键9
							  break;					  
				 case(0x16):  disp_string(0,1," *");  //按键*
							  break;					  
				 case(0x0d):  disp_string(0,1," #");  //按键#
							  break;					  
				 case(0x18):  disp_string(0,1," ^");  //按键^
							  break;					  
				 case(0x08):  disp_string(0,1," <");  //按键 <
							  break;					  
		         case(0x1c):disp_string(0,1,"ok");  //按键ok
							  break;					  
				 case(0x5a):  disp_string(0,1," >");  //按键>
							  break;					  
				 case(0x52):  disp_string(0,1," V");  //按键V
							  break;					  
	}  
	  DispOneChar(4,1,IrValue[2]/16+0X30);	//显示命令码  为什么要加上0x30
	  
	  if( IrValue[2]%16<10 )   // 小于10的是数字0-9,否则是字母，abcdefg
      {DispOneChar(5,1,IrValue[2]%16+0X30);}  //显示十位值  为什么要加上0x30
		else
			{DispOneChar(5,1,IrValue[2]%16+0X37);}  //显示个位值  为什么要加上0x37
			
		LED1= ~LED1;
		delay(40000);

	
}}

void ReadIr() interrupt 0   //外部中断零服务程序
{
	unsigned int  j,k;
	unsigned char  err;
	Time=0;					 
	delay(700);	//7ms
	if(IRIN==0)		//确认是否真的接收到正确的信号
	{	 
		
		err=1000;				//1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/	
		while((IRIN==0)&&(err>0))	//等待前面9ms的低电平过去  		
		{			
			delay(1);
			err--;
		} 
		if(IRIN==1)			//如果正确等到9ms低电平
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //等待4.5ms的起始高电平过去
			{
				delay(1);
				err--;
			}
			for(k=0;k<4;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					err=60;		
					while((IRIN==0)&&(err>0))//等待信号前面的560us低电平过去
					{
						delay(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //计算高电平的时间长度。
					{
						delay(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k表示第几组数据
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//用完时间要重新赋值							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}
 
//---------延时函数-------------
void  delay(unsigned int i)
{
	while(i--);
}
