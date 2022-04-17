#include "key.h"
#include "gpio.h"

unsigned int key_flag=0;

void key_scan()
{
	static unsigned int i=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
	{
		i++;
		if(i==2)
		{
			i=0;
			key_flag=11;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
	{
		i++;
		if(i==2)
		{
			i=0;
			key_flag=21;
		}
	}
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
	{
		i++;
		if(i==2)
		{
			i=0;
			key_flag=12;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
	{
		i++;
		if(i==2)
		{
			i=0;
			key_flag=22;
		}
	}
}

