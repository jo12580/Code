/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c.h
Author:      Version:   Date:2019.10.10
Description: 罗列了I2C操作函数。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#include "stm32l0xx_hal.h"

#define SCL	GPIO_PIN_6	//PA8
#define SDA	GPIO_PIN_7	//PB4

/**
  * @brief I2C起始信号
  * @param None
  * @retval None
  */
void I2CStart2(void);

/**
  * @brief I2C结束信号
  * @param None
  * @retval None
  */
void I2CStop2(void);

/**
  * @brief I2C等待确认信号
  * @param None
  * @retval None
  */
unsigned char I2CWaitAck2(void);

/**
  * @brief I2C发送确认信号
  * @param None
  * @retval None
  */
void I2CSendAck2(void);

/**
  * @brief I2C发送非确认信号
  * @param None
  * @retval None
  */
void I2CSendNotAck2(void);

/**
  * @brief I2C发送一个字节
  * @param cSendByte 需要发送的字节
  * @retval None
  */
void I2CSendByte2(unsigned char cSendByte);

/**
  * @brief I2C接收一个字节
  * @param None
  * @retval 接收到的字节
  */
unsigned char I2CReceiveByte2(void);

#endif
