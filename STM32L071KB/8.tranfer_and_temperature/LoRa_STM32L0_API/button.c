#include "stm32l0xx.h"
//#include "stm32l0xx_hal.h"
#include "timer.h"


void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	
    __HAL_RCC_GPIOC_CLK_ENABLE();//PC14
    GPIO_Initure.Pin = GPIO_PIN_14; //PC4
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
	
		GPIO_Initure.Pin = GPIO_PIN_15;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_Initure);
}


uint8_t BUTTON_Scan(void)
{
    uint8_t key_num = 0;
    static uint8_t key_state = 0;
    switch(key_state)
    {
    case 0:
    {
        if(1 != HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14))
            key_state = 1;
        break;
    }
    case 1:
    {
        if(1 != HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14))
        {
            key_state = 2;
            key_num = 1;
            break;
        }
        else
        {
            key_state = 0;
            break;
        }
    }
    case 2:
    {
        if(1 == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14))
            key_state = 0;
        break;
    }
    }
    return key_num;
}



