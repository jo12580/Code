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

void delay(uint i);
uchar KeyDown(void);

#endif
