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

radio_handle_t  Radio_handle;


extern char g_lcdLine_1st_line[16];
extern char g_lcdLine_2st_line[16];
extern uint8_t Display_Temper_Num;
extern float temper;
extern RTC_TimeTypeDef NowTime;


uint8_t Cmd_B1[5] = {0x0A, 0x0B, 0xB1, 0xEE, 0xEF};
uint8_t Cmd_B3[5] = {0x0A, 0x0B, 0xB2, 0xEE, 0xEF};
uint8_t Cmd_B2[5] = {0x0A, 0x0B, 0xB3, 0xEE, 0xEF};

uint8_t Cmd_B1_Replay[7] = {'C', 'm', 'd', 'B', '1', 'o', 'k'};



uint8_t Cmd_B2_Replay[24] = {"CmdB2ok,00:00:00-00.00C"};
uint8_t Cmd_B3_Replay[7] = {'C', 'm', 'd', 'B', '3', 'o', 'k'};

uint8_t Cmd_Buff[255] = {0};
uint8_t Radio_Rxbuff[255] = {0};
uint8_t Radio_Txbuff[255] = {0x31, 0x32, 0x33, 0x34};
uint16_t Radio_RxLength = 0;

uint8_t Temper_Replay[80] = {0};


void delay_us(uint32_t time)
{
    while(time--);
}


void TaskRecv_Init(void)
{

    Radio_handle = radio_zm4xx_inst_init(spi_recv_byte, spi_send_byte, zm4xx_rst_pin_set, zm4xx_sel_pin_set, HAL_Delay, delay_us); //LORA初始化

    if (Radio_handle != NULL)
    {
        radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
    }//lora初始化完成
}

uint8_t Compare_Cmd(uint8_t *recv_cmd_buf, uint8_t recv_buf_num)
{
    uint8_t cmd_Num = 0;
    uint8_t index = 0;

    switch (recv_cmd_buf[2])
    {
    case 0xB1:
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B1[index])
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

    case 0xB2:
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B2[index])
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

    case 0xB3:
        for ( ; index < recv_buf_num; index++)
        {
            if (recv_cmd_buf[index] == Cmd_B3[index])
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

    default :
        ;
        break;
    }
    return cmd_Num;
}

void Save_temper(void)
{
    char save_buf[15] = {0};
    uint8_t index = 0;

    sprintf(save_buf, "%d:%d:%d-%.2fC", NowTime.Hours, NowTime.Minutes, NowTime.Seconds, temper);
    for ( ; index < 15; index++)
    {
        Cmd_B2_Replay[8 + index] = save_buf[index];
    }
}

void Task_Recv(void)
{
    uint8_t index = 0;
    uint8_t Recv_Cmd_Num = 0;

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);

    if ( radio_buf_recv(Radio_handle, Radio_Rxbuff, &Radio_RxLength) == RADIO_RET_OK)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        for ( ; index < Radio_RxLength; index++)
        {
            Cmd_Buff[index] = Radio_Rxbuff[index];
        }

        Recv_Cmd_Num = Compare_Cmd(Cmd_Buff, Radio_RxLength);

        switch (Recv_Cmd_Num)
        {
        case 1 :
            Save_temper();
            radio_buf_send(Radio_handle, Cmd_B1_Replay, 7);
            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            break;

        case 2 :
            radio_buf_send(Radio_handle, Cmd_B2_Replay, 24);
            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            break;

        case 3 :
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
            radio_buf_send(Radio_handle, Cmd_B3_Replay, 7);

            radio_mode_set(Radio_handle, RX_MODE);//进入接收模式
            break;

        default :
            ;
            break;
        }
    }

}


