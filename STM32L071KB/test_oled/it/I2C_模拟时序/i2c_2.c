#include "i2c_2.h"

#define DELAY_TIME 20

//I2C总线内部延时函数
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

//配置SDA引脚为输入模式
void SDA_Input_Mode2(void)
{

}

//配置SDA引脚为输出模式
void SDA_Output_Mode2(void)
{

}

//SDA引脚输出
void SDA_Output2(uint16_t val )
{

}

//SCL引脚输出
void SCL_Output2( uint16_t val )
{

}

//读取SDA引脚状态
uint8_t SDA_Input2(void)
{

}

//I2C总线启动信号
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

//I2C总线停止信号
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

//等待应答
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
    SCL_Output2(0);
    SDA_Output_Mode2();
    delay1(DELAY_TIME);
    return SUCCESS;
}

//发送应答
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

//发送非应答
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

//发送一个字节数据
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

//接收一个字节数据
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
