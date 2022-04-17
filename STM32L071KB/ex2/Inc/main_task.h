/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: main_task.h
Author:      Version:   Date:2019.10.10
Description: ����main_task.c�ļ�������ͷ�ļ���
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
  * @brief �弶�����ʼ��
  * @param None
  * @retval None
  */
void Task_BrdInit(void);

/**
  * @brief �弶����ִ��
  * @param None
  * @retval None
  */
void Task_Main(void);


#endif
