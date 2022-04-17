/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: sts30.c
Author:      Version:   Date:2019.10.10
Description: 温度传感器的功能函数
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#include "sts30.h"
#include "i2c_2.h"


float Get_Temperature(void)
{
    uint8_t H_VALUE, L_VALUE, CRC_VALUE;
    uint16_t T_VALUE; //转换完成的数字值
    float temperature;//转换完成的温度值

    I2CStart2();
    I2CSendByte2(0x4A << 1);
    I2CWaitAck2();
    I2CSendByte2(0x24);//不使能时钟伸缩
    I2CWaitAck2();
    I2CSendByte2(0x0b);//中重复性测量精度
    I2CWaitAck2();
    I2CStop2();
    HAL_Delay(2);//scl free,最小为1ms

    I2CStart2();
    I2CSendByte2(0x4A << 1 | 1);
    I2CWaitAck2();
    I2CStop2();
    HAL_Delay(2);//scl free,最小为1ms

    I2CStart2();
    I2CSendByte2(0x4A << 1 | 1);
    I2CWaitAck2();


    H_VALUE = I2CReceiveByte2();
    I2CSendAck2();
    L_VALUE = I2CReceiveByte2();
    I2CSendAck2();
    CRC_VALUE = I2CReceiveByte2();
    I2CSendNotAck2();
    I2CStop2();

    if (CRC_VALUE)
    {
        ;
    }
    T_VALUE = H_VALUE << 8 | L_VALUE;
    temperature = -45 + 175.0 * T_VALUE / 65535;

    return (temperature);
}




