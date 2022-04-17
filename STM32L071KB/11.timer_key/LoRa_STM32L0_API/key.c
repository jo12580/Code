#include "key.h"

unsigned int KEY_flag=0;
uint16_t count=0;
uint8_t data1[20]="key_flag == short\r\n";
uint8_t data2[20]="key_flag == long\r\n";

void Key_scan()
{
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)==0)
	{
		count++;
	}
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)==1)
	{
		if(count>1&&count<100)
		{
			KEY_flag=1;
			HAL_UART_Transmit(&huart2,data1,20,100);
		}	
		if(count>=100)
		{
			KEY_flag=2;
			HAL_UART_Transmit(&huart2,data2,20,100);
		}
		count=0;
	}
}
void Led_scan()
{
	if(KEY_flag==1)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	}
	if(KEY_flag==2)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	}
}

//void Key_scan()
//{
//	uint8_t  val = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);//1 表示高电平，0表示低电平。低电平时按下	
//	if(val==0)			//按键按下 记录按下的时间
//	{
//		count++;
//	}
//	if(val==1)					//按键松开 计数值清零并返回按下的时间值
//	{
//		if(count>100)//按下经过1s  10*10ms=100ms
//		{
//			KEY_flag = 2;			//长按
//			HAL_UART_Transmit(&huart2,data1,20,100);
//		}
//		else if(count>10 && count <100)//表示超过10ms
//		{
//			KEY_flag = 1;			//短按
//			HAL_UART_Transmit(&huart2,data2,20,100);
//		}
////		else
////		{
////			//消抖时间
////		}
//		count=0;
//	}
//}

//void Led_scan()
//{
//	switch(KEY_flag)
//	{
//		case 1:
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);break;
//		case 2:
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);break;
//		default:;break;
//	}
//}

///*
//	每10ms进入一次该函数，检查一下引脚状态
//	可以用定时器，也可以用查询
//*/
//unsigned int KEY_flag=0;
//uint16_t count=0;
//uint8_t data1[20]="key_flag == long\r\n";
//uint8_t data2[20]="key_flag == short\r\n";

//void Key_scan(void)
//{
//	//1 表示高电平，0表示低电平。低电平时按下	
//	uint8_t  val = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);
//	if(val==0)//按键按下 记录按下的时间
//	{
//		count++;
//	}
//	else //按键松开 计数值清零并返回按下的时间值
//	{
//		if(count>100)//按下经过1s  10*10ms=100ms
//		{
//    		//长按
//			KEY_flag = 2;
////			printf("key_flag == long\r\n");
//			HAL_UART_Transmit(&huart2,data1,20,100);
//		}
//		else if(count>10 && count <100)//表示超过10ms
//		{
//				//短按
//			KEY_flag = 1;
////			printf("key_flag == short\r\n");
//			HAL_UART_Transmit(&huart2,data2,20,100);
//		}
//		else
//		{
//			//消抖时间
//		}
//		count=0;
//	}
//}

///*
//*依据标志位改变LED的状态
//*/
//void Led_scan()
//{
//	switch(KEY_flag)
//	{
//		case 1:
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
//		break;
//		case 2:
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
//		break;
//		default:
//			break;
//	}
//}

