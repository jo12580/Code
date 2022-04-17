#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>
#include "predefine.h"

sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;

void init_lcd1602(void);
void locate(uchar x,uchar y);
void display_str(uchar *str);
void write_data(uchar dat);

#endif