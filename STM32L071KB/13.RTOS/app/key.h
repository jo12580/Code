#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"
#include "tim.h"

extern unsigned int KEY_flag;

void key_scan(void);
void Led_scan(void);

#endif
