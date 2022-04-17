#include "i2c_2.h"


#define DELAY_TIME 2

//I2C总线内部延时函数
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

//配置SDA引脚为输入模式
void SDA_Input_Mode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = SDA;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//配置SDA引脚为输出模式
void SDA_Output_Mode(void)
{
GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = SDA;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//SDA引脚输出
void SDA_Output(uint16_t val )
{
	if(val)
		HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_RESET);
}

//SCL引脚输出
void SCL_Output( uint16_t val )
{
	if(val)
		HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_SET);
	else 
		HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_RESET);
}

//读取SDA引脚状态
uint8_t SDA_Input(void)
{
	return HAL_GPIO_ReadPin(GPIOB, SDA);
}

//I2C总线启动信号
void I2CStart(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//I2C总线停止信号
void I2CStop(void)
{
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(1);
    delay1(DELAY_TIME);

}

//等待应答
unsigned char I2CWaitAck(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode();
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    while(SDA_Input())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode();
            I2CStop();
            return ERROR;
        }
    }
    SCL_Output(0);
    SDA_Output_Mode();
    delay1(DELAY_TIME);
    return SUCCESS;
}

//发送应答
void I2CSendAck(void)
{
    SDA_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//发送非应答
void I2CSendNotAck(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//发送一个字节数据
void I2CSendByte(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output(0);
        delay1(DELAY_TIME);
        SDA_Output(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//接收一个字节数据
unsigned char I2CReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input();
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode();
    return cR_Byte;
}
