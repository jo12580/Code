/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c.h
Author:      Version:   Date:2019.10.10
Description: 罗列了I2C操作函数。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __KEY_TASK_H
#define __KEY_TASK_H

#include "stm32l0xx_hal.h"
#include "key.h"
#include "sx127x_lora.h"
#include "string.h"
#include "stdio.h"


void Task_Key(void);
void TaskKey_init(void);


#endif
