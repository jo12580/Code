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
        if (MISO_READ()) {
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