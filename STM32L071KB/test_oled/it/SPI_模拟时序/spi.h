#ifndef __SPI_H
#define __SPI_H

//SPI总线引脚定义
#define CLK_L() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define CLK_H() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define MOSI_L() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define MOSI_H() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define MISO_READ() 	HAL_GPIO_ReadPin((GPIOA, GPIO_PIN_6)

//接口函数
void delay_us(uint32_t time);
void spi_init(void);
void spi_send_byte(uint8_t byte);
uint8_t spi_recv_byte(void);
    	

#endif  


