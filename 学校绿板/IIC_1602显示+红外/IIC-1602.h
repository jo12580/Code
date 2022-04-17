/********************************************
功能：测试串口LCD1602，是否正常
日期：2019年7月29日
作者：
*********************************************/


/********************************************
MODE:1602J
LCD IC:AIP31068L
接口：IIC
MCU:W78E516D   12MHZ
*********************************************/

/********************************************
Verification: Jiande Su
modify date:2019-07-28
experience：杜帮线接触不良，消耗了一个下午时间; 经验是液晶一定要判断初始化正常了没有。
MCU: STC90C52   12MHZ
*********************************************/
/*
1.VSS--------------0V
2.VDD--------------5V
3.V0
4.SCL_1602--------------P3.0
5.SDA_1602--------------P3.1
6.CS---------------NC
7~14.  NC
15.BLA-------------5V
16.BLK-------------0V


sbit sda_1602=P1^1;  //16脚串口芯片定义，直接放在并口座上
sbit scl_1602=P1^0;  //16脚串口芯片定义，直接放在并口座上

*/



#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

sbit sda_1602=P3^7;
sbit scl_1602=P3^6;



/*=======================================================
 有关串口LCD1602的函数声明
=======================================================*/
void delay1ms(int i); //
void Lcd1602Iinitial();
void transfer(uchar data1);  //主要使用
void start_flag();
void stop_flag();

void Disp_XY( char posx,char posy);
void DispOneChar(uchar x,uchar y,uchar Wdata);    //主要使用
void disp_string(uchar x,uchar y,uchar *s);       //主要使用
