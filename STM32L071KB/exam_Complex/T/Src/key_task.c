/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: key_task.c
Author:      Version:   Date:2019.10.10
Description: 按键任务
Others:
History:

1. Date:
Author:
Modification:
*************************************************/


#include "key_task.h"

extern radio_handle_t  Radio_handle;
extern char g_lcdLine_1st_line[16];
extern char g_lcdLine_2st_line[16];
extern uint8_t g_Key_Value;

uint8_t g_UI_1_Flag = 0;
uint8_t g_UI_2_Flag = 0;

uint8_t Cmd_B1[5] = {0x0A, 0x0B, 0xB1, 0xEE, 0xEF};
uint8_t Cmd_B2[5] = {0x0A, 0x0B, 0xB2, 0xEE, 0xEF};
uint8_t Cmd_B3[5] = {0x0A, 0x0B, 0xB3, 0xEE, 0xEF};


uint8_t Radio_State = 0;

void TaskKey_init(void)
{
    Keyboard_Init();

    g_UI_1_Flag = 1;  // 初始界面1
    g_UI_2_Flag = 0;
}

void Task_Key(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);

    switch (g_Key_Value)
    {
    case B1:
        if (g_UI_1_Flag == 1)
        {
            radio_buf_send(Radio_handle, Cmd_B1, 5);
            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            sprintf(g_lcdLine_1st_line, "Command: B1     ");
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        }
        g_Key_Value = 0;
        break;

    case B2:
        if (g_UI_1_Flag == 1)
        {
            radio_buf_send(Radio_handle, Cmd_B2, 5);
            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            sprintf(g_lcdLine_1st_line, "Command: B2     ");
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        }
        g_Key_Value = 0;
        break;

    case B3:
        if (g_UI_1_Flag == 1)
        {
            radio_buf_send(Radio_handle, Cmd_B3, 5);
            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            sprintf(g_lcdLine_1st_line, "Command: B3     ");
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        }
        g_Key_Value = 0;
        break;

    case B4:
        if ((g_UI_1_Flag == 1) && (g_UI_2_Flag == 0))
        {
            g_UI_1_Flag = 0;
            g_UI_2_Flag = 1;
            sprintf(g_lcdLine_1st_line, "Temperature  ");
            sprintf(g_lcdLine_2st_line, "00:00:00-00.00C");
        }
        else if ((g_UI_1_Flag == 0) && (g_UI_2_Flag == 1))
        {
            g_UI_1_Flag = 1;
            g_UI_2_Flag = 0;
            sprintf(g_lcdLine_1st_line, "Command: None  ");
            sprintf(g_lcdLine_2st_line, "Status : None  ");
        }
        g_Key_Value = 0;
        break;

    default :
        ;
        break;
    }
}
