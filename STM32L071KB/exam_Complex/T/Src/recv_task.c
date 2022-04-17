/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: recv_task.c
Author:      Version:   Date:2019.10.10
Description: 接收到的数据处理
Others:
History:

1. Date:
Author:
Modification:
*************************************************/


#include "recv_task.h"

extern uint8_t g_UI_1_Flag ;
extern uint8_t g_UI_2_Flag ;
extern char g_lcdLine_1st_line[16];
extern char g_lcdLine_2st_line[16];


radio_handle_t  Radio_handle;
uint8_t Cmd_B1_Replay[7] = {'C', 'm', 'd', 'B', '1', 'o', 'k'};
uint8_t Cmd_B3_Replay[7] = {'C', 'm', 'd', 'B', '3', 'o', 'k'};
uint8_t Cmd_B2_Replay[7] = {'C', 'm', 'd', 'B', '2', 'o', 'k'};

uint8_t Cmd_Buff[255] = {0};
uint8_t Radio_Rxbuff[255] = {0};
uint8_t Radio_Txbuff[255] = {0x30, 0x30, 0x30, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A};
uint16_t Radio_RxLength = 0;

uint8_t Temper_Replay[80] = {0};

void delay_us(uint32_t time)
{
    while(time--);
}

void TaskRecv_Init(void)
{
    Radio_handle = radio_zm4xx_inst_init(spi_recv_byte, spi_send_byte, zm4xx_rst_pin_set, zm4xx_sel_pin_set, HAL_Delay, delay_us); //LORA初始化
}

uint8_t Compare_Cmd(uint8_t *recv_cmd_buf, uint8_t recv_buf_num)
{
    uint8_t cmd_Num = 0;
    uint8_t index = 0;

    switch (recv_cmd_buf[4])
    {
    case '1':
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B1_Replay[index])
            {

            }
            else
            {
                cmd_Num = 0;
                continue;
            }
        }
        cmd_Num = 1;
        break;

    case '2':
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B2_Replay[index])
            {

            }
            else
            {
                cmd_Num = 0;
                continue;
            }
        }
        cmd_Num = 2;

        break;

    case '3':
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B3_Replay[index])
            {

            }
            else
            {
                cmd_Num = 0;
                continue;
            }
        }
        cmd_Num = 3;
        break;
    }

    return cmd_Num;
}

/**
 * \brief  无线模块接收中断服务函数
 */
uint8_t g_recv_byte = 0;
uint8_t g_recv_flag = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    g_recv_flag = 1;
}

void Task_Recv(void)
{
    uint8_t index = 0;
    uint8_t Recv_Cmd_Num = 0;
    int  ret = 0;

    if (g_recv_flag == 1)
    {
        g_recv_flag = 0;
        ret = radio_buf_recv(Radio_handle, Radio_Rxbuff, &Radio_RxLength);  /*  接收数据  */
        g_recv_byte = Radio_RxLength;
    }
		
    if ((g_recv_byte > 0) && (ret == RADIO_RET_OK))
    {
        for ( ; index < Radio_RxLength; index++)
        {
            Cmd_Buff[index] = Radio_Rxbuff[index];
        }
        Recv_Cmd_Num = Compare_Cmd(Cmd_Buff, Radio_RxLength);
        g_recv_byte = 0;
    }


    if (g_UI_1_Flag == 1)
    {
        switch (Recv_Cmd_Num)
        {
        case 1 :
            sprintf(g_lcdLine_2st_line, "Status :Cmd1 ok");
            break;

        case 2 :
            sprintf(g_lcdLine_2st_line, "Status :Cmd2 ok");
            for (index = 0; index < Radio_RxLength - 8; index++)
            {
                Temper_Replay[index] = Cmd_Buff[index + 8];
            }
            break;

        case 3 :
            sprintf(g_lcdLine_2st_line, "Status :Cmd3 ok");
            break;

        default :
            ;
            break;
        }
    }
    if (g_UI_2_Flag == 1)
    {
        for (index = 0; index < 15; index++)
        {
            g_lcdLine_2st_line[index] = Temper_Replay[index];
        }
    }
}


