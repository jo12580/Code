#include"key.h"
#include "SysTick.h"

void key_init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY 1,2,3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

unsigned int key2_scan()
{
	unsigned int a=0;
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0)
		{
			a=1;
			while(!(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0));
		}
		else 
		{
			a=0;
		}
	}
	return a;
}

unsigned int key1_scan()
{
	unsigned int a=0;
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)==0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)==0)
		{
			a=1;
			while(!(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)==0));
		}
		else 
		{
			a=0;
		}
	}
	return a;
}

unsigned int key0_scan()
{
	unsigned int a=0;
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0)
		{
			a=1;
			while(!(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0));
		}
		else 
		{
			a=0;
		}
	}
	return a;
}


