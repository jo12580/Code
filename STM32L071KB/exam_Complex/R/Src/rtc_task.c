/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: key_task.c
Author:      Version:   Date:2019.10.10
Description: °´¼üÈÎÎñ
Others:
History:

1. Date:
Author:
Modification:
*************************************************/


#include "rtc_task.h"
extern RTC_HandleTypeDef hrtc;
extern char g_lcdLine_1st_line[16];
extern char g_lcdLine_2st_line[16];

RTC_TimeTypeDef NowTime;
RTC_DateTypeDef NowDate;


void TaskRtc_Init(void)
{

}

void Task_Rtc(void)
{

    HAL_RTC_GetTime(&hrtc, &NowTime, RTC_FORMAT_BIN);

    HAL_RTC_GetDate(&hrtc, &NowDate, RTC_FORMAT_BIN);

    sprintf(g_lcdLine_1st_line, "%d : %d : %d ", NowTime.Hours, NowTime.Minutes, NowTime.Seconds);
}
