/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c_2.h
Author:      Version:   Date:2019.10.10
Description: 温度传感器的I2C操作的驱动。注意两个驱动时序略有差别。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __I2C_2_H
#define __I2C_2_H

#include "stm32l0xx_hal.h"

#define SCL2	GPIO_PIN_6	//PB6
#define SDA2	GPIO_PIN_7	//PB7

//
void I2CStart2(void);
void I2CStop2(void);
unsigned char I2CWaitAck2(void);
void I2CSendAck2(void);
void I2CSendNotAck2(void);
void I2CSendByte2(unsigned char cSendByte);
unsigned char I2CReceiveByte2(void);

#endif
