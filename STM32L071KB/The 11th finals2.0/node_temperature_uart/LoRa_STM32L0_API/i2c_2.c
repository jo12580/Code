/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: i2c_2.c
Author:      Version:   Date:2019.10.10
Description: Universal extension interface I2C driver.
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#include "i2c_2.h"

#define DELAY_TIME 20

float temperature()
{
	uint8_t hight,low,crc;
	uint16_t sum;
	float value;
	I2CStart2();
	I2CSendByte2(0x4A <<1);
	I2CWaitAck2();
	
	I2CSendByte2(0x27);
	I2CWaitAck2();
	
	I2CSendByte2(0x37);
	I2CWaitAck2();
	I2CStop2();
	HAL_Delay(2);
	
	I2CStart2();
	I2CSendByte2(0x4A <<1|1);
	I2CWaitAck2();
	
	hight=I2CReceiveByte2();
	I2CSendAck2();
	low=I2CReceiveByte2();
	I2CSendAck2();
	crc=I2CReceiveByte2();
	I2CSendNotAck2();
	I2CStop2();
	
	sum=hight<<8|low;
	value=-45+175.0*sum/65535;
	return value;
}

/**
  * @brief SDA线输入模式配置
  * @param None
  * @retval None
  */
void SDA_Input_Mode2()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = SDA2;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出模式配置
  * @param None
  * @retval None
  */
void SDA_Output_Mode2()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = SDA2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SDA_Output2( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= SDA2;
    }
    else
    {
        GPIOB->BRR |= SDA2;
    }
}

/**
  * @brief SCL线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SCL_Output2( uint16_t val )
{
    if ( val )
    {

        GPIOB->BSRR |= SCL2;
    }
    else
    {
        GPIOB->BRR |= SCL2;
    }
}

/**
  * @brief SDA输入一位
  * @param None
  * @retval GPIO读入一位
  */
uint8_t SDA_Input2(void)
{
    return HAL_GPIO_ReadPin(GPIOB, SDA2);
}


/**
  * @brief I2C的短暂延时
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C起始信号
  * @param None
  * @retval None
  */
void I2CStart2(void)
{
    SDA_Output2(1);
    delay1(DELAY_TIME);
    SCL_Output2(1);
    delay1(DELAY_TIME);
    SDA_Output2(0);
    delay1(DELAY_TIME);
    SCL_Output2(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C结束信号
  * @param None
  * @retval None
  */
void I2CStop2(void)
{
    SCL_Output2(0);
    delay1(DELAY_TIME);
    SDA_Output2(0);
    delay1(DELAY_TIME);
    SCL_Output2(1);
    delay1(DELAY_TIME);
    SDA_Output2(1);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C等待确认信号
  * @param None
  * @retval None
  */
unsigned char I2CWaitAck2(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode2();
    delay1(DELAY_TIME);
    SCL_Output2(1);
    delay1(DELAY_TIME);
    while(SDA_Input2())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode2();
            I2CStop2();
            return ERROR;
        }
    }
    SDA_Output_Mode2();
    SCL_Output2(0);
    delay1(DELAY_TIME);
    return SUCCESS;
}

/**
  * @brief I2C发送确认信号
  * @param None
  * @retval None
  */
void I2CSendAck2(void)
{
    SDA_Output2(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output2(1);
    delay1(DELAY_TIME);
    SCL_Output2(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送非确认信号
  * @param None
  * @retval None
  */
void I2CSendNotAck2(void)
{
    SDA_Output2(1);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output2(1);
    delay1(DELAY_TIME);
    SCL_Output2(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送一个字节
  * @param cSendByte 需要发送的字节
  * @retval None
  */
void I2CSendByte2(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output2(0);
        delay1(DELAY_TIME);
        SDA_Output2(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output2(1);
        delay1(DELAY_TIME);
    }
    SCL_Output2(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C接收一个字节
  * @param None
  * @retval 接收到的字节
  */
unsigned char I2CReceiveByte2(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode2();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output2(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output2(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input2();
    }
    SCL_Output2(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode2();
    return cR_Byte;
}
