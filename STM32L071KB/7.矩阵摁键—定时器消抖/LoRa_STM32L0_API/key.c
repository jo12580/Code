#include "key.h"

unsigned int KEY_flag;

void key_scan()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		KEY_flag=11;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		KEY_flag=21;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		KEY_flag=12;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		KEY_flag=22;
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		KEY_flag=13;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		KEY_flag=23;
}

void Led_scan()
{
	if(KEY_flag==11)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	}
	if(KEY_flag==21)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

//void key_scan()
//{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
//	{
//		key_i++;
//		count=3;
//	}
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))	
//	{
//		key_i++;
//		count=2;
//	}
//	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==0&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==0)
//	{
//		if(count>1&&count<10)
//		{
//			if(count==3)
//				KEY_flag=111;
//			if(count==2)
//				KEY_flag=211;
//		}
//		if(count>=10)
//		{
//			if(count==3)	
//				KEY_flag=112;
//			if(count==2)	
//				KEY_flag=212;
//		}
//		key_i=0;
//		count=0;
//	}
//}


