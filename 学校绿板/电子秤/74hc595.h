#include "reg52.h"
#include "intrins.h"
 
sbit DATA_595=P2^2; //����������
sbit SCK_595=P2^1;  //����ʱ��������
sbit RCK_595=P2^0;  //���ݲ����������

//sbit DATA_595_2=P3^5; //����������
//sbit SCK_595_2=P3^6;  //����ʱ��������
//sbit RCK_595_2=P3^7;  //���ݲ����������

void HC595SendData(unsigned char SendVal,unsigned char ctr); 
void display_date(unsigned char year,month,day);
