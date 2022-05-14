#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H

#include "main.h"
#include "crc.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

void main_init(void);
void main_task(void);

#endif
