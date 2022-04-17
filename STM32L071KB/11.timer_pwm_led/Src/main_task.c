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
#include "stm32l0xx_hal.h"
extern TIM_HandleTypeDef htim21;
extern TIM_HandleTypeDef htim3;
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

    OLED_ShowString(0, 0, (unsigned char *)"PWM  TEST       ", 16);
    USERKey_Init();
}

/**
  * @brief 板级任务执行
  * @param None
  * @retval None
  */
uint8_t g_PWM = 0;
void Task_Main(void)
{
    char lcdLine_2st_line[16];

    UserKey_Fun()->KEY_Scan();

    if(UserKey_Fun()->value == USER)
    {
        UserKey_Fun()->value = NOKEY;
        g_PWM += 10;
			
				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,g_PWM);
			
//        TIM3->CCR3 = g_PWM * 10;
//        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        if (g_PWM == 110)
        {
            g_PWM = 0;
//            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
        }
        sprintf(lcdLine_2st_line, "PWM : %d         ", g_PWM);

        OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
    }
}
