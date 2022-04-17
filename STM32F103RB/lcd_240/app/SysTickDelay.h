#ifndef __SysTickDelay_H
#define __SysTickDelay_H

#include "stm32f1xx_it.h"
#include "main.h"

typedef unsigned           char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

void SysTick_Initaize(uint8_t SYSCLK);
void delay_us(uint32_t nus);

#endif 
