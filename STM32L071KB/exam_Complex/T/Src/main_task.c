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


char g_lcdLine_1st_line[16];
char g_lcdLine_2st_line[16];
extern uint8_t g_OLED_Flag;
extern uint8_t g_Radio_Recv_Flag;
extern TIM_HandleTypeDef htim6;







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


    spi_init();
    zm4xx_pin_init();
    TaskKey_init();
    TaskRecv_Init();
    HAL_TIM_Base_Start_IT(&htim6);

    sprintf(g_lcdLine_1st_line, "Command: None");
    sprintf(g_lcdLine_2st_line, "Status : None");


}

/**
  * @brief 板级任务执行
  * @param None
  * @retval None
  */
void Task_Main(void)
{

    Task_Key();

    if (g_OLED_Flag == 1)
    {
        g_OLED_Flag = 0;
        OLED_ShowString(0, 0, (uint8_t *)g_lcdLine_1st_line, 16);
        OLED_ShowString(0, 2, (uint8_t *)g_lcdLine_2st_line, 16);
    }

    Task_Recv();
}
