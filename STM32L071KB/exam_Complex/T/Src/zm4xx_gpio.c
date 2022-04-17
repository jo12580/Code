#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"

void zm4xx_pin_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_4; //PA4
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); //复位引脚初始化为高电平
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //片选引脚初始化为高电平
}

void zm4xx_rst_pin_set(uint8_t value)
{
    if(value)
    {
        GPIOA->BSRR = GPIO_PIN_9;
    }
    else
    {
        GPIOA->BRR = GPIO_PIN_9;
    }
}

void zm4xx_sel_pin_set(uint8_t value)
{
    if(value)
    {
        GPIOA->BSRR = GPIO_PIN_4;
    }
    else
    {
        GPIOA->BRR = GPIO_PIN_4;
    }
}


