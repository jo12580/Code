#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"


void delay_us(uint32_t time)//����������ȷ�����ڿ�����Ҫ�޸�,��ʱʱ�����Ϊ25us//LORA��Ҫ
{
    while(time--);
}

//��ʱ nms����ȷ��ʱ
//nms:Ҫ��ʱ�� ms ��
void delay_ms(uint32_t nms)//LORA��Ҫ
{
    HAL_Delay(nms);
}











