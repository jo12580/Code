/*
���ܣ����Դ���LCD1602���Ƿ�����
���ڣ�2019��7��29��
���ߣ�

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

sbit LED1=P1^4;  //��Ҫ�����Գ�����

/*=======================================================
 �йش���LCD1602�ĺ�������
=======================================================*/
void delay1ms(int i); //
void Lcd1602Iinitial();
void transfer(int data1);  //��Ҫʹ��
void start_flag();
void stop_flag();

void Disp_XY( char posx,char posy);              //��ʾ����
void disp_string(uchar x,uchar y,uchar *s);      //��ʾ�ַ���
void DispOneChar(uchar x,uchar y,uchar Wdata);   //��ʾһ���ַ�
void DispNumber(uchar x,uchar y,uint number);    //��ʾ��ֵ

