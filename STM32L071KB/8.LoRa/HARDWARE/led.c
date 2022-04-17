#include"led.h"


void LED_On(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //开启GPIOC时钟
    GPIO_Initure.Pin = GPIO_PIN_15;
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLDOWN;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
}

void LED_Off(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //开启GPIOC时钟
    GPIO_Initure.Pin = GPIO_PIN_15;
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLDOWN;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
}



