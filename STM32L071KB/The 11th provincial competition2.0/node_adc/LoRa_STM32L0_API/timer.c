#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"


void delay_us(uint32_t time)//待定，不精确，后期可能需要修改,延时时间大致为25us//LORA需要
{
    while(time--);
}

//延时 nms，精确延时
//nms:要延时的 ms 数
void delay_ms(uint32_t nms)//LORA需要
{
    HAL_Delay(nms);
}











