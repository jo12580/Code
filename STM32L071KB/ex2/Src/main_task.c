/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: main_task.c
Author:      Version:   Date:2019.10.10
Description: 单片机执行的任务。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#include "main_task.h"


/**
  * @brief 板级任务初始化
  * @param None
  * @retval None
  */
void Task_BrdInit(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);

    OLED_PowerControl(ENABLE);

    HAL_Delay(200);
    OLED_Init();
    OLED_Clear();

    OLED_ShowString(0, 0, (unsigned char *)"                ", 16);
    OLED_ShowString(0, 2, (unsigned char *)"                ", 16);

}

/**
  * @brief 板级任务执行
  * @param None
  * @retval None
  */
void Task_Main(void)
{
    char lcdLine_1st_line[16];
    char lcdLine_2st_line[16];

    sprintf(lcdLine_1st_line, "GXCT IOT       ");
    sprintf(lcdLine_2st_line, "OLED Test      ");

    OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
    OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);

}
