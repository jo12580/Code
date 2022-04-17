#include "spi.h"


//SPI总线内部延时函数
void delay_us(uint32_t time)
{
	while(time--){
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	}
}

//SPI相关引脚初始化配置
void spi_init(void)
{
    //MISO引脚初始化
    //MOSI引脚初始化
    //CLK引脚初始化
    //CS引脚初始化
	GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

//发送一个字节数据
void spi_send_byte (uint8_t send_data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        delay_us(1);
        CLK_L();
        if((send_data << i) & 0x80){
            delay_us(1);
            MOSI_H(); 
        } else {
            delay_us(1);
            MOSI_L(); 
        }

        delay_us(1);
        CLK_H();
    }

    delay_us(1);
    CLK_L();
    MOSI_L();
}

//接收一个字节数据
uint8_t spi_recv_byte (void)
{
    uint8_t i, temp;

    temp = 0;
    for (i = 0; i < 8; i++) {
        CLK_L();
        delay_us(1);
        temp = (temp << 1);
        CLK_H(); 
        delay_us(1);
        if (MISO_READ()) 
				{
            temp++;
        } else { 
            temp = temp + 0;
        }

        delay_us(1);
    }

    CLK_L();
    delay_us(1);

    return temp;
}