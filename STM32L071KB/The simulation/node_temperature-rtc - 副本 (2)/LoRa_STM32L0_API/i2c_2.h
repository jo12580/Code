/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c_2.h
Author:      Version:   Date:2019.10.10
Description: Description: Universal extension interface I2C driver.
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __I2C_2_H
#define __I2C_2_H

#include "stm32l0xx_hal.h"

#define SCL2	GPIO_PIN_6
#define SDA2	GPIO_PIN_7

//
void I2CStart2(void);
void I2CStop2(void);
unsigned char I2CWaitAck2(void);
void I2CSendAck2(void);
void I2CSendNotAck2(void);
void I2CSendByte2(unsigned char cSendByte);
unsigned char I2CReceiveByte2(void);

#endif
