#include "key.h"

unsigned int key=0;
unsigned int key_num,key_a=0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_14)
	{
		key++;
		if(key==2)
			key=0;
	}
//	if(GPIO_Pin==GPIO_PIN_6)
//	{
//		key_a++;
//		if(key_a==500)
//		{
//			switch(key_num)
//			{
//				case 1:
//				{
//					key=11;
//				}break;
//				case 2:
//				{
//					key=12;
//				}break;
//				case 3:
//				{
//					key=13;
//				}break;
//				default : key=0;break;
//			}
//			key_a=0;
//		}
//	}
//	if(GPIO_Pin==GPIO_PIN_7)
//	{
//		key_a++;
//		if(key_a==500)
//		{
//			switch(key_num)
//			{
//				case 1:
//				{
//					key=21;
//				}break;
//				case 2:
//				{
//					key=22;
//				}break;
//				case 3:
//				{
//					key=23;
//				}break;
//				default : key=0;break;
//			}
//			key_a=0;
//		}
//	}
}

void key_scan()
{
	key_num=1;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	key_num=2;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	key_num=3;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}
