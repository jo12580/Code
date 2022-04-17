#ifndef __I2C_2_H
#define __I2C_2_H

//I2C总线引脚定义
//#define SCL2	GPIO_PIN_6
//#define SDA2	GPIO_PIN_7

//接口函数
void I2CStart2(void);
void I2CStop2(void);
unsigned char I2CWaitAck2(void);
void I2CSendAck2(void);
void I2CSendNotAck2(void);
void I2CSendByte2(unsigned char cSendByte);
unsigned char I2CReceiveByte2(void);

#endif
