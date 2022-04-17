#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

void KEY_Init(void);
u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex);
u8 KEY_Scan(void);

#endif

