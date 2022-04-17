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

#ifndef __RECV_TASK_H
#define __RECV_TASK_H

#include "stm32l0xx_hal.h"
#include "stdio.h"
#include "sx127x_lora.h"
#include "spi.h"
#include "zm4xx_gpio.h"
#include "timer.h"


void Task_Recv(void);
void TaskRecv_Init(void);


#endif
