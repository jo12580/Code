#ifndef __ZX4XX_GPIO_H
#define __ZX4XX_GPIO_H

#include "stm32l0xx.h"
#include "stdlib.h"

void zm4xx_sel_pin_set(uint8_t value);
void zm4xx_rst_pin_set(uint8_t value);
void zm4xx_pin_init(void);

#endif



