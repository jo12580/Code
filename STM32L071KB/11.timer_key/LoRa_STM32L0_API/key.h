#ifndef __KEY_H
#define __KEY_H

#include "stm32l0xx_hal.h"
#include "usart.h"

extern void Key_scan(void);
extern void Led_scan(void);
extern unsigned int KEY_flag;

#endif
