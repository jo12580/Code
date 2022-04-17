#include "ds18b20.h"	//包含头文件
#include "delay.h"
#include "lcd1602.h"

uint temperature;

/*---------------单线总线操作流程-----------------------------------------

*  单线总线工作过程中的协议如下：
*  初始化，ROM操作命令，存储器操作命令，处理数据。
*  每次要对存储器操作前，都必须先进行初始化，然后进行ROM操作，最后才能对
*  存储器进行操作。
------------------------------------------------------------------------*/

/*************************************************************************
* 名        称 ：void ds18b20_reset(void)
* 功        能 ：初始化DS18B20
* 输        入 ：NULL
* 全 局 变  量 ：NULL  
* 返   回   值 ：NULL
* 调        用 ：NULL
* 编 写 日  期 ：2008-7-23 
* 最后修改日期 ：2008-7-23
*************************************************************************/
uchar ds18b20_reset(void)		   //复位函数
{
	uchar x=0;

	DQ=1;    			
	delay_time(2);    	//稍做延时
	DQ=0;    			//单片机将DQ拉低  
	delay_time(80);   	//精确延时 大于 480us
	DQ=1;    			//拉高总线
	delay_time(5);
	x=DQ;
	while(!DQ);
	delay_time(40);   	
	return x;
}

uchar ds18b20_read_slot()	 //读一位
{
	uchar bdat;

	DQ=1;
	DQ=0;    		  
	DQ=1;    		
	bdat=DQ;
	delay_time(4);
	return bdat;
}

uchar  ds18b20_read_byte(void)		//读一个字节函数
{
	uchar i,dat;
	
	dat=0;
	for(i=0;i<8;i++)	dat|=(ds18b20_read_slot()<<i);
	return dat;
}

void ds18b20_write_slot(uchar bdat)		 //写一位
{
	DQ=1;
	DQ=0;    		  
	DQ=bdat;    		
	delay_time(4);
	DQ=1;
}

void ds18b20_write_byte(uchar dat)//写一个字节函数 0011 0011
{
	uchar i;
	
	for(i=0;i<8;i++)	ds18b20_write_slot(dat&(1<<i));
}

void ds18b20_set_resolution(uchar res)		//分辨率设置函数
{
	while(ds18b20_reset());  				// 初始化
	ds18b20_write_byte(skip_rom); 			// 跳过读序号列号的操作
	ds18b20_write_byte(write_scrctchpad); 	// 写暂存器
	ds18b20_write_byte(0x1e); 				// 报警上限
	ds18b20_write_byte(0x0a); 				// 报警下限
	ds18b20_write_byte((res<<5)|0x1f); 		// 分辨率
}

/*************************************************************************
* 名        称 ：int read_temperature(void)
* 功        能 ：初始化及修改18b20中时间数据
* 输        入 ：NULL
* 全 局 变  量 ：NULL  
* 返   回   值 ：NULL
* 调        用 ：NULL
* 编 写 日  期 ：2008-7-23 
* 最后修改日期 ：2008-7-23
*************************************************************************/
void ds18b20_read_temperature(void)
{
	uchar  a,b;

	while(ds18b20_reset());  				// 初始化
	ds18b20_write_byte(skip_rom); 			// 跳过读序号列号的操作
	ds18b20_write_byte(convert_t);		 	// 启动温度转换
	
	while(!ds18b20_read_slot());
		
	while(ds18b20_reset());  				// 初始化
	ds18b20_write_byte(skip_rom); 			// 跳过读序号列号的操作
	ds18b20_write_byte(read_scrctchpad); 	// 读暂存器

	a=ds18b20_read_byte();
	b=ds18b20_read_byte();
	temperature=b;
	temperature<<=8;
	temperature|=a;
}

/*----------------------------
   温度显示子程序
-----------------------------*/
void ds18b20_display_temperature(void)
{
    uchar i,j;

	lcd1602_locate(8,1);						    //符号位
    if(temperature&0x80==0) lcd1602_write_data('+');
	else 
	{
		temperature=~temperature;
		temperature++;
		lcd1602_write_data('-');
	}

	i=(uchar)(temperature>>4); 						 //整数部分
    j=(uchar)((temperature&0x000f)*6.25);	         //小数部分

	lcd1602_write_data(i/10+0x30);
	lcd1602_write_data(i%10+0x30);
    lcd1602_locate(12,1);
	lcd1602_write_data(j/10+0x30);
	lcd1602_write_data(j%10+0x30);
}