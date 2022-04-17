#ifndef __TIM_H
#define __TIM_H

#include <stdint.h>

//void delay_us(uint32_t nus);
void delay_us(uint32_t time);
void delay_ms(uint32_t nms);
void TIM2_Init(void);

#endif


