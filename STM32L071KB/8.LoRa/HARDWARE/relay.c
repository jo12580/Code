#include"relay.h"

void RELAY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); //¿ªÆôGPIOCÊ±ÖÓ
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}



