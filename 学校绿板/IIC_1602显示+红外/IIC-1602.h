/********************************************
���ܣ����Դ���LCD1602���Ƿ�����
���ڣ�2019��7��29��
���ߣ�
*********************************************/


/********************************************
MODE:1602J
LCD IC:AIP31068L
�ӿڣ�IIC
MCU:W78E516D   12MHZ
*********************************************/

/********************************************
Verification: Jiande Su
modify date:2019-07-28
experience���Ű��߽Ӵ�������������һ������ʱ��; ������Һ��һ��Ҫ�жϳ�ʼ��������û�С�
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


sbit sda_1602=P1^1;  //16�Ŵ���оƬ���壬ֱ�ӷ��ڲ�������
sbit scl_1602=P1^0;  //16�Ŵ���оƬ���壬ֱ�ӷ��ڲ�������

*/



#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

sbit sda_1602=P3^7;
sbit scl_1602=P3^6;



/*=======================================================
 �йش���LCD1602�ĺ�������
=======================================================*/
void delay1ms(int i); //
void Lcd1602Iinitial();
void transfer(uchar data1);  //��Ҫʹ��
void start_flag();
void stop_flag();

void Disp_XY( char posx,char posy);
void DispOneChar(uchar x,uchar y,uchar Wdata);    //��Ҫʹ��
void disp_string(uchar x,uchar y,uchar *s);       //��Ҫʹ��
