#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
	
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit IRIN=P1^1;
sbit a=P3^2;

u8 IrValue[6];
u8 Time;

u8 DisplayData[8];
u8 code smgduan[19]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 
0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码

void delay(u16 i)
{
	while(i--);	
}
void Delay500us()		//@12.000MHz
{
	unsigned char i, j;

	i = 6;
	j = 211;
	do
	{
		while (--j);
	} while (--i);
}

/*******************************************************************************
* 函数名         :DigDisplay()
* 函数功能		 :数码管显示函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Display()
{
	u16 i;
	for(i=0;i<=2;i++)
	{
		P2=0xc0;
    switch(i)
		{
			case 0:P0=0x01;break;
			case 1:P0=0x02;break;
			case 2:P0=0x04;break;
		}
		P2=0xe0;
		P0= DisplayData[i];
		Delay500us();	
		P0=0xff;	
	}
}


/*******************************************************************************
* 函数名         : IrInit()
* 函数功能		   : 初始化红外线接收
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void IrInit()
{
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA=1;	//打开总中断
	IRIN=1;//初始化端口
		a=IRIN;
}

void main()
{	
	IrInit();
	
	while(1)
	{	
			a=IRIN;
		DisplayData[0] = smgduan[IrValue[2]/16];
		DisplayData[1] = smgduan[IrValue[2]%16];
		DisplayData[2] = smgduan[16];
	  Display();	
	}		
}

/*******************************************************************************
* 函数名         : ReadIr()
* 函数功能		   : 读取红外数值的中断函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void ReadIr() interrupt 0
{
	u8 j,k;
	u16 err;
		a=IRIN;
	Time=0;	
	IRIN=1;//初始化端口	
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