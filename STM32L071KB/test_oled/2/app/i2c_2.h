#ifndef __I2C_2_H
#define __I2C_2_H

//I2C总线引脚定义
#define SCL	GPIO_PIN_8	//PA8
#define SDA	GPIO_PIN_4	//PB4
#include "gpio.h"

//接口函数
void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void OLED_Write(uint8_t ucType, uint8_t ucData);  	
#endif
