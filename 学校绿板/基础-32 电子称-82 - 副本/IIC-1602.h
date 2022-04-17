/*
功能：测试串口LCD1602，是否正常
日期：2019年7月29日
作者：

1.VSS--------------0V
2.VDD--------------5V
3.V0
4.SCL_1602
5.SDA_1602
*/


#include <reg52.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

sbit sda_1602=P3^7;
sbit scl_1602=P3^6;

sbit LED1=P1^4;  //主要做测试程序用

/*=======================================================
 有关串口LCD1602的函数声明
=======================================================*/
void delay1ms(int i); //
void Lcd1602Iinitial();
void transfer(int data1);  //主要使用
void start_flag();
void stop_flag();

void Disp_XY( char posx,char posy);              //显示坐标
void disp_string(uchar x,uchar y,uchar *s);      //显示字符串
void DispOneChar(uchar x,uchar y,uchar Wdata);   //显示一个字符
void DispNumber(uchar x,uchar y,uint number);    //显示数值

