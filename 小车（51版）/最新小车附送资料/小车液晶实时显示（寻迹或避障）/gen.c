//巍巍机器人小卖部提供//
//淘宝地址：http://shop58277238.taobao.com//
//欢迎您的使用//

#include<reg52.h>
#include <intrins.h>
sbit    sce = P0^3;  //片选
sbit    res = P0^4;  //复位,0复位
sbit    dc  = P0^2;  //1写数据，0写指令
sbit    sdin = P0^1;  //数据
sbit    sclk = P0^0;  //时钟
sbit    ledd = P0^5;  //背景灯
sbit    m11 = P1^0;   //电机控制
sbit    m12 = P1^1;  //电机控制
sbit     m21 = P1^2;  //电机控制
sbit     m22 = P1^3;  //电机控制
sbit   RightSenser = P1^4;   //信号输入端口
sbit   LeftSenser = P1^5;   //信号输入端口

unsigned char code shuzi[]={


/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*--  文字:  2  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,

/*--  文字:  3  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,



	
};

unsigned char code maohao[]={
/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00


};
unsigned char code jiaohao[]={



/*--  文字:  -  --50*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x00
};

unsigned char code hanzi[]=
{
/*--  文字:  系  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x02,0x22,0xB2,0xAA,0x66,0x62,0x22,0x11,0x4D,0x81,0x01,0x01,0x00,0x00,
0x00,0x40,0x21,0x13,0x09,0x05,0x41,0x81,0x7F,0x01,0x05,0x09,0x13,0x62,0x00,0x00,

/*--  文字:  统  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x20,0x30,0x2C,0xA3,0x60,0x10,0x84,0xC4,0xA4,0x9D,0x86,0x84,0xA4,0xC4,0x84,0x00,
0x20,0x22,0x23,0x12,0x12,0x92,0x40,0x30,0x0F,0x00,0x00,0x3F,0x40,0x41,0x70,0x00,

/*--  文字:  已  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0xE2,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7E,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3C,0x10,0x00,

/*--  文字:  启  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x00,0xF8,0x48,0x48,0x48,0x49,0x4E,0x4A,0x48,0x48,0x48,0x78,0x00,0x00,
0x40,0x20,0x18,0x07,0x00,0x7E,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x7E,0x00,0x00,

/*--  文字:  动  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x20,0x24,0x24,0xE4,0x24,0x24,0x24,0x20,0x10,0x10,0xFF,0x10,0x10,0xF0,0x00,0x00,
0x08,0x1C,0x0B,0x08,0x0C,0x05,0x4E,0x24,0x10,0x0C,0x03,0x20,0x40,0x3F,0x00,0x00,

/*--  文字:  向  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0xFC,0x04,0x04,0xE6,0x25,0x24,0x24,0x24,0xE4,0x04,0x04,0xFC,0x00,0x00,
0x00,0x00,0x7F,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x0F,0x20,0x40,0x3F,0x00,0x00,

/*--  文字:  右  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x08,0x08,0x88,0xC8,0x38,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
0x04,0x02,0x01,0x00,0x7F,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x7F,0x00,0x00,0x00,

/*--  文字:  转  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0xC8,0xA8,0x9C,0xEB,0x88,0x88,0x88,0x40,0x48,0xF8,0x4F,0x48,0x48,0x48,0x40,0x00,
0x08,0x08,0x04,0xFF,0x04,0x04,0x00,0x02,0x0B,0x12,0x22,0xD2,0x0E,0x02,0x00,0x00,

/*--  文字:  弯  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x42,0x62,0x5A,0x52,0x42,0x7E,0x43,0x42,0x7E,0x42,0x4A,0xD2,0x32,0x02,0x00,
0x00,0x00,0x00,0x0F,0x09,0x09,0x09,0x09,0x09,0x09,0x49,0x89,0x49,0x38,0x00,0x00,

/*--  文字:  向  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0xFC,0x04,0x04,0xE6,0x25,0x24,0x24,0x24,0xE4,0x04,0x04,0xFC,0x00,0x00,
0x00,0x00,0x7F,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x0F,0x20,0x40,0x3F,0x00,0x00,

/*--  文字:  左  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x08,0x08,0x08,0x08,0xC8,0x3F,0x0A,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
0x40,0x20,0x50,0x4C,0x43,0x41,0x41,0x41,0x7F,0x41,0x41,0x41,0x41,0x40,0x40,0x00,

/*--  文字:  转  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0xC8,0xA8,0x9C,0xEB,0x88,0x88,0x88,0x40,0x48,0xF8,0x4F,0x48,0x48,0x48,0x40,0x00,
0x08,0x08,0x04,0xFF,0x04,0x04,0x00,0x02,0x0B,0x12,0x22,0xD2,0x0E,0x02,0x00,0x00,

/*--  文字:  弯  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x42,0x62,0x5A,0x52,0x42,0x7E,0x43,0x42,0x7E,0x42,0x4A,0xD2,0x32,0x02,0x00,
0x00,0x00,0x00,0x0F,0x09,0x09,0x09,0x09,0x09,0x09,0x49,0x89,0x49,0x38,0x00,0x00,

/*--  文字:  全  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x80,0x40,0x60,0x50,0x48,0x44,0xC3,0x44,0x48,0x50,0x70,0x60,0x20,0x00,0x00,
0x00,0x40,0x40,0x44,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x46,0x44,0x60,0x40,0x00,

/*--  文字:  速  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x42,0xCC,0x00,0x04,0xE4,0x24,0x24,0xFF,0x24,0x24,0x24,0xE4,0x04,0x00,0x00,
0x40,0x20,0x1F,0x20,0x48,0x49,0x45,0x43,0x7F,0x41,0x43,0x45,0x4D,0x40,0x40,0x00,

/*--  文字:  前  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x08,0xE8,0xA8,0xA9,0xAE,0xEA,0x08,0x08,0xC8,0x0C,0x0B,0xEA,0x08,0x08,0x00,
0x00,0x00,0x7F,0x04,0x24,0x44,0x3F,0x00,0x00,0x1F,0x40,0x80,0x7F,0x00,0x00,0x00,

/*--  文字:  进  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x80,0x82,0x9C,0x88,0x00,0x88,0x88,0xFF,0x88,0x88,0x88,0xFF,0x88,0x88,0x80,0x00,
0x00,0x40,0x20,0x1F,0x20,0x50,0x4C,0x43,0x40,0x40,0x40,0x5F,0x40,0x40,0x40,0x00,

/*--  文字:  巍  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x50,0xD6,0xF4,0xD4,0x54,0x44,0xF7,0x5C,0x54,0xF4,0x54,0x54,0xF6,0x00,0x00,
0x84,0x4D,0x36,0x15,0x2C,0xC5,0x00,0x83,0x62,0x1E,0x03,0x7E,0x52,0x5F,0x70,0x00,

/*--  文字:  巍  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x50,0xD6,0xF4,0xD4,0x54,0x44,0xF7,0x5C,0x54,0xF4,0x54,0x54,0xF6,0x00,0x00,
0x84,0x4D,0x36,0x15,0x2C,0xC5,0x00,0x83,0x62,0x1E,0x03,0x7E,0x52,0x5F,0x70,0x00,

/*--  文字:  机  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x08,0xC8,0xFF,0x48,0x88,0x08,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,
0x04,0x03,0x00,0xFF,0x00,0x41,0x30,0x0C,0x03,0x00,0x00,0x00,0x3F,0x40,0x78,0x00,

/*--  文字:  器  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x40,0x4F,0x49,0x49,0xC9,0xCF,0x70,0xC0,0xCF,0x49,0x59,0x69,0x4F,0x00,0x00,
0x02,0x02,0x7E,0x45,0x45,0x44,0x7C,0x00,0x7C,0x44,0x45,0x45,0x7E,0x06,0x02,0x00,

/*--  文字:  人  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0xC2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x01,0x06,0x0C,0x18,0x30,0x60,0x20,0x00,

/*--  文字:  欢  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x14,0x24,0x44,0x84,0x64,0x1C,0x20,0x18,0x0F,0xE8,0x08,0x08,0x28,0x18,0x08,0x00,
0x20,0x10,0x4C,0x43,0x43,0x2C,0x20,0x10,0x0C,0x03,0x06,0x18,0x30,0x60,0x20,0x00,

/*--  文字:  迎  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x41,0xCE,0x04,0x00,0xFC,0x04,0x02,0x02,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x40,0x20,0x1F,0x20,0x40,0x47,0x42,0x41,0x40,0x5F,0x40,0x42,0x44,0x43,0x40,0x00,

/*--  文字:  您  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x80,0x40,0x30,0xFC,0x03,0x90,0x68,0x06,0x04,0xF4,0x04,0x24,0x44,0x8C,0x04,0x00,
0x00,0x20,0x38,0x03,0x38,0x40,0x40,0x49,0x52,0x41,0x40,0x70,0x00,0x09,0x30,0x00,

/*--  文字:  的  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0xF8,0x8C,0x8B,0x88,0xF8,0x40,0x30,0x8F,0x08,0x08,0x08,0x08,0xF8,0x00,0x00,
0x00,0x7F,0x10,0x10,0x10,0x3F,0x00,0x00,0x00,0x03,0x26,0x40,0x20,0x1F,0x00,0x00,

/*--  文字:  使  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x20,0xF0,0x1C,0x07,0xF2,0x94,0x94,0x94,0xFF,0x94,0x94,0x94,0xF4,0x04,0x00,
0x00,0x00,0x7F,0x00,0x40,0x41,0x22,0x14,0x0C,0x13,0x10,0x30,0x20,0x61,0x20,0x00,

/*--  文字:  用  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,
0x80,0x40,0x30,0x0F,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,






};

void delay_1ms(void)//1ms延时函数
{
	unsigned int i;
	for (i=0;i<500;i++)	  ;
;
}

/*--------------------------------------------
LCD_write_byte: 使用SPI接口写数据到LCD
输入参数：dt：写入的数据；
command ：写数据/命令选择；
编写日期：20080918 
----------------------------------------------*/
void LCD_write_byte(unsigned char dt, unsigned char command)
{
	unsigned char i; 
	sce=0;	
	dc=command;	
	for(i=0;i<8;i++)
	{ 
		if(dt&0x80)
			sdin=1;
		else
			sdin=0;
		dt=dt<<1;		
		sclk=0; 		
		sclk=1; 
	}	
	dc=1;	
	sce=1;	
	sdin=1;
}
/*---------------------------------------
LCD_init: 3310LCD初始化
编写日期：20080918 
-----------------------------------------  */
void LCD_init(void)
{
	res=0;  	
  	delay_1ms();
  	res=1;  
	LCD_write_byte(0x21,0);//初始化Lcd,功能设定使用扩充指令
	LCD_write_byte(0xd0,0);//设定液晶偏置电压
	LCD_write_byte(0x20,0);//使用基本指令
	LCD_write_byte(0x0C,0);//设定显示模式，正常显示
}
/*-------------------------------------------
LCD_set_XY: 设置LCD坐标函数
输入参数：X：0－83  Y：0－5
编写日期：20080918 
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// column
	LCD_write_byte(0x80 | X, 0);// row
} 
/*------------------------------------------
LCD_clear: LCD清屏函数
编写日期：20080918 
--------------------------------------------*/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);
				
		} 
	}
}



/*---------------------------------------------
LCD_write_shu: 显示8（宽）*16（高）点阵列数字字母符号等半角类
输入参数：c：显示的字符；
编写日期：20080918 
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1);
	}	 	
}
/*---------------------------------------------
LCD_write_hanzi: 显示16（宽）*16（高）点阵列汉字等半角类
输入参数：c：显示的字符；
编写日期：20080918 
-----------------------------------------------*/
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<16;i++) 
	{
		LCD_write_byte(hanzi[c*32+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=16; i<32;i++) 
	{
		LCD_write_byte(hanzi[c*32+i],1);
	}	
}


main()
{
    unsigned int k;  


    for(k=0;k<25000;k++);



	res=0;
	ledd=0;
	res=1;	  	
  	LCD_init();  //初始化LCD模块 
	LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,17);  //巍
	LCD_write_hanzi(2,0,18);  //巍
	LCD_write_hanzi(4,0,19);  //机
	LCD_write_hanzi(6,0,20);  //器			
	LCD_write_hanzi(8,0,21);  //人
	LCD_write_hanzi(0,2,22);  //欢
	LCD_write_hanzi(2,2,23);  //迎
	LCD_write_hanzi(4,2,24);  //您
	LCD_write_hanzi(6,2,25);  //的		
	LCD_write_hanzi(8,2,26);  //使
	LCD_write_hanzi(0,4,27);  //用


	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);

    LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,0);  //系
	LCD_write_hanzi(2,0,1);  //统	
	LCD_write_hanzi(4,0,3);  //起	
	LCD_write_hanzi(6,0,4);  //动

	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);

	LCD_clear(); //清屏幕	

	LCD_write_shu(4,2,2);  //3

	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);

	LCD_clear(); //清屏幕
	LCD_write_shu(4,2,1);  //2
	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);


	LCD_clear(); //清屏幕
	LCD_write_shu(4,2,0);  //1
	for(k=0;k<25000;k++);
    for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
 




	while(1)		
	{
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);
	for(k=0;k<25000;k++);

 m11 = 1;
 m12 = 1;
 m21 = 1; 
 m22 = 1; 


             	
	if((!RightSenser) && LeftSenser)   // 如果右边假，左边真
              	{
				
	         	 m11 = 0;
                 m12 = 1;

            	LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,5);  //向
	LCD_write_hanzi(2,0,6);  //右	
	LCD_write_hanzi(4,0,7);  //转	
	LCD_write_hanzi(6,0,8);  //弯		




                for (k=0; k<100; k++);   // 延时
            	}

   	if(RightSenser && (!LeftSenser))  // 如果左边假，右边真
             	{
                 m21 = 0; 
                  m22 = 1; 

            	LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,9);  //向
	LCD_write_hanzi(2,0,10);  //左	
	LCD_write_hanzi(4,0,11);  //转	
	LCD_write_hanzi(6,0,12);  //弯			

	
					for (k=0; k<100; k++);  // 延时
                  	}

   	if((RightSenser) && (LeftSenser))  // 如果左边假，右边真
             	{
 m11 = 0;
 m12 = 1;
 m21 = 0; 
 m22 = 1; 
 
            	LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,13);  //全	
	LCD_write_hanzi(2,0,14);  //速		
	LCD_write_hanzi(4,0,15);  //前		
	LCD_write_hanzi(6,0,16);  //进	
            
	
					for (k=0; k<100;k++);  // 延时

            	}
	if((!RightSenser) && (!LeftSenser))  // 如果左边假，右边真
             	{
 m11 = 0;
 m12 = 1;
 m21 = 0; 
 m22 = 1; 

            	LCD_clear(); //清屏幕	
	LCD_write_hanzi(0,0,13);  //全	
	LCD_write_hanzi(2,0,14);  //速		
	LCD_write_hanzi(4,0,15);  //前		
	LCD_write_hanzi(6,0,16);  //进	
            
	
					for (k=0; k<100;k++);  // 延时

            	}

	}
	}

