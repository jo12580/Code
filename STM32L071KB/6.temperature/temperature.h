/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c.h
Author:      Version:   Date:2019.10.10
Description: ������I2C����������
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
  * @brief I2C��ʼ�ź�
  * @param None
  * @retval None
  */
void I2CStart2(void);

/**
  * @brief I2C�����ź�
  * @param None
  * @retval None
  */
void I2CStop2(void);

/**
  * @brief I2C�ȴ�ȷ���ź�
  * @param None
  * @retval None
  */
unsigned char I2CWaitAck2(void);

/**
  * @brief I2C����ȷ���ź�
  * @param None
  * @retval None
  */
void I2CSendAck2(void);

/**
  * @brief I2C���ͷ�ȷ���ź�
  * @param None
  * @retval None
  */
void I2CSendNotAck2(void);

/**
  * @brief I2C����һ���ֽ�
  * @param cSendByte ��Ҫ���͵��ֽ�
  * @retval None
  */
void I2CSendByte2(unsigned char cSendByte);

/**
  * @brief I2C����һ���ֽ�
  * @param None
  * @retval ���յ����ֽ�
  */
unsigned char I2CReceiveByte2(void);

#endif
