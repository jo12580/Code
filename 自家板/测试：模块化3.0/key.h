#ifndef _KEY_H_
#define _KEY_H_

#include<reg52.h>

#define GPIO_KEY P1

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

#define GPIO_DIG P0

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;


void delay(uint i);
uchar KeyDown(void);
void key();

#endif
