#include "stm32f10x.h"                  // Device header
#include"led.h"
#include"key.h"
#include "SysTick.h"

int main()
{
	unsigned int c=0,b=0;
	SysTick_Init(72);
	led_init();
	key_init();
	while(1)
	{
		c=key0_scan();
		b=key2_scan();
		if(b==1)
		{			
//			GPIO_SetBits(GPIOA, GPIO_Pin_6);
//			delay_ms(1000);
//			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
//			delay_ms(1000);
//			GPIO_SetBits(GPIOA, GPIO_Pin_6);
//			delay_ms(1000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
			delay_ms(1000);
		}
		if(c==1)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
//			delay_ms(1000);
//			GPIO_SetBits(GPIOA, GPIO_Pin_7);
//			delay_ms(1000);
//			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
//			delay_ms(1000);
//			GPIO_SetBits(GPIOA, GPIO_Pin_7);
//			delay_ms(1000);
		}
	}
}
