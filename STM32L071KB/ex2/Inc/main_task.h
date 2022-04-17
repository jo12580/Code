/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: main_task.h
Author:      Version:   Date:2019.10.10
Description: 包含main_task.c文件函数的头文件。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H

#include "stm32l0xx_hal.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"


/**
  * @brief 板级任务初始化
  * @param None
  * @retval None
  */
void Task_BrdInit(void);

/**
  * @brief 板级任务执行
  * @param None
  * @retval None
  */
void Task_Main(void);


#endif
