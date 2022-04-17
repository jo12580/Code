#include "reg52.h"
#include "intrins.h"
 
sbit DATA_595=P2^2; //串行数据线
sbit SCK_595=P2^1;  //串行时钟线输入
sbit RCK_595=P2^0;  //数据并行输出控制

//sbit DATA_595_2=P3^5; //串行数据线
//sbit SCK_595_2=P3^6;  //串行时钟线输入
//sbit RCK_595_2=P3^7;  //数据并行输出控制

void HC595SendData(unsigned char SendVal,unsigned char ctr); 
void display_date(unsigned char year,month,day);
