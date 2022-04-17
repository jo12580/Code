#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "zm4xx_gpio.h"
#include "timer.h"

#define CLK_L() GPIOA->BRR = GPIO_PIN_5
#define CLK_H() GPIOA->BSRR = GPIO_PIN_5
#define MOSI_L() GPIOA->BRR = GPIO_PIN_7
#define MOSI_H() GPIOA->BSRR = GPIO_PIN_7
#define MISO_READ() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)

void spi_init(void)
{
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


void spi_send_byte (uint8_t send_data)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        delay_us(1);
        CLK_L();                                                        /* clk=0;                       */
        if((send_data << i) & 0x80)
        {
            delay_us(1);
            MOSI_H();                                                    /* set 1;                       */
        }
        else
        {
            delay_us(1);
            MOSI_L();                                                    /* set 0;                       */
        }

        delay_us(1);
        CLK_H();                                                        /* clk=1;                       */
    }

    delay_us(1);
    CLK_L();
    MOSI_L();                                                            /* set 0;                       */
}

uint8_t spi_recv_byte (void)
{
    uint8_t i, temp;

    temp = 0;
    for (i = 0; i < 8; i++)
    {
        CLK_L();                                                        /* clk=0;                       */
        delay_us(1);
        temp = (temp << 1);
        CLK_H();                                                         /* clk=1;                       */
        delay_us(1);
        if (MISO_READ())
        {
            temp++;
        }
        else                                                                   /* set 1;                       */
        {
            temp = temp + 0;
        }

        delay_us(1);
    }

    CLK_L();
    delay_us(1);

    return temp;
}




