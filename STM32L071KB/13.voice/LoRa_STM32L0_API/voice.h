#include "stm32l0xx_hal.h"


//sbit rest=P2^7; //语音芯片复位引脚定义
//sbit  dat=P2^6;  //语音芯片数据线引脚定义
//sbit busy=P2^5;  //语音芯片---引脚定义
void voice(unsigned char i);
void  delay_us(unsigned int i);
