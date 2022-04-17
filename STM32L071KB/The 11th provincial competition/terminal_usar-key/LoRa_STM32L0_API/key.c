#include "key.h"
#include "gpio.h"

unsigned int key_flag;
unsigned int key_i=0;

//void key_scan()
//{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=11;
//			key_i=0;
//		}
//	}
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=21;
//			key_i=0;
//		}
//	}
//	
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=12;
//			key_i=0;
//		}		
//	}
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=22;
//			key_i=0;
//		}
//	}	
//	
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=13;
//			key_i=0;
//		}		
//	}
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
//	{
//		key_i++;
//		if(key_i>1)
//		{
//			key_flag=23;
//			key_i=0;
//		}
//	}
//}

void key_scan()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
	{
			key_flag=11;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
	{
			key_flag=21;
	}
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
	{
			key_flag=12;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
	{
			key_flag=22;
	}	
	
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
	{
			key_flag=13;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
	{
			key_flag=23;
	}
}
