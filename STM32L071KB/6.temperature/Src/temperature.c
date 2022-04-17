#include "temperature.h"

#define DELAY_TIME 2

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


//float Get_Temperature(void)
//{
//	uint8_t H_VALUE,L_VALUE,CRC_VALUE;
//	uint16_t  T_VALUE;	//转换完成的数字值
//	float temperature; 	//转换完成的温度值
//	
//	I2CStart2();
//	I2CSendByte2(0x4A << 1);
//	I2CWaitAck2();
//	I2CSendByte2(0x24); //不使能时钟伸缩
//	I2CWaitAck2();
//	I2CSendByte2(0x0b); //中重复性测量精度
//	I2CWaitAck2();
//	I2CStop2();
//	delay2(DELAY_TIME); //scl free,最小为1ms
//	
//	I2CStart2();
//	I2CSendByte2(0x4A<< 1 | 1) ;
//	I2CWaitAck2();
//	I2CStop2();
//	delay2(DELAY_TIME);  //scl free,最小为1ms
//	
//	I2CStart2();
//	I2CSendByte2(0x4A<< 1 | 1) ;
//	I2CWaitAck2();
//	
//	H_VALUE =I2CReceiveByte2();
//	I2CSendAck2();
//	L_VALUE =I2CReceiveByte2();
//	I2CSendAck2();
//	CRC_VALUE = I2CReceiveByte2();
//	I2CSendNotAck2();
//	I2CStop2();
//	
//	if (CRC_VALUE)
//	{
//		;
//	}
//	T_VALUE = H_VALUE<< 8| L_VALUE;
//	temperature = -45 +175.0* T_VALUE /65535;
//	return temperature;
//}


void SDA_Input_Mode2(void)
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
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
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
        //HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_SET);
    }
    else
    {
        GPIOB->BRR |= SDA2;
        //HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_RESET);
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
        //HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_SET);
    }
    else
    {
        GPIOB->BRR |= SCL2;
        //HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_RESET);
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
    unsigned int i;
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
