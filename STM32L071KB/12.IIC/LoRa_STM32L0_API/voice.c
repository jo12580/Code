#include<voice.h>

void  delay_us(unsigned int i)
{
	while(i--);
}

void voice(unsigned char i)
{	
	unsigned char a;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		delay_us(50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);	
		delay_us(50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);	
    delay_us(50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		delay_us(50);
	for(a=0;a<i;a++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		delay_us(50);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		delay_us(50);   
	 }
//   while (!busy);   
//delay_us(30000);		 
}
