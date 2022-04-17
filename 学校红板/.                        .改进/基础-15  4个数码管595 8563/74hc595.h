#ifndef __74hc595_H__
#define __74hc595_H__

#include "reg52.h"
#include "intrins.h"
 
sbit DATA_595=P2^2; //串行数据线
sbit RCK_595=P2^0;  //数据并行输出控制
sbit SCK_595=P2^1;  //串行时钟线输入

void HC595SendData(unsigned char SendVal,unsigned char ctr);      
void display_time(unsigned char hour_2,min_2,sec_2,week_2);      
void display_date(unsigned char year_2,month_2,day_2);
void display_temperature(int temp_int_2,temp_float2int_2);

#endif