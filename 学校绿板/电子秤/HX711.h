#ifndef __HX711_H__
#define __HX711_H__


#include <reg52.h>
#include <intrins.h>


//sbit HX711_DOUT=P0^6; 
//sbit HX711_SCK =P0^7; 
sbit HX711_DOUT=P2^3; 
sbit HX711_SCK =P2^4; 

//�������߱�������
extern   void Delay__hx711_us(void);  //�ٶ�������Ƶ����11.0592mhz
unsigned long HX711_Read(void);
//unsigned long  fun_total(unsigned long  * array);  //ȡ10��ֵ��ȥ�����ֵ����Сֵ�����8��ƽ��

#endif