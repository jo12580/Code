#include "stm32f10x.h"
#include "led.h"

void delay(int i)
{
	while(i--);
}

int main()
{
	LED_Init();
	while(1)
	{
		GPIO_ResetBits(LED_PORT,LED_PIN);//µ„¡¡D1
		delay(10000);
		GPIO_SetBits(LED_PORT,LED_PIN);
	}
}
