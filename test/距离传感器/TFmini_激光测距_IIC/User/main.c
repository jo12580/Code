#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "iic.h"
#include <stdlib.h>
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{	
	uint8_t buf[7];
	__I2C_Status_TypeDef i2CStatus;
	int16_t Dist = 0;
	uint16_t Strength = 0;
	uint8_t Mode = 0;

	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //配置中断分组
	LED_Init();  									 //初始化LED
	USART1_Init(115200);							 //初始化串口
	User_I2C_Init();								 //初始化TFmini激光雷达模块

	delay_ms(100);
	printf("into while loop!\r\n");
	while(1)
	{
		i2CStatus = I2C_RecvBytes(0x10, 0x102, buf, 7, 100);
		if(i2CStatus == I2C_OK)
		{
			Mode = buf[6];
			Dist = buf[2] | (buf[3] << 8);
			Strength = buf[4] | (buf[5] << 8);
			
			printf("Slave 0x%x: Dist[%5d], Strength[%5d], Mode[%d].\r\n", \
					0x10, Dist, Strength, Mode);
		}
		else
		{
			printf("Err: I2C_RecvBytes.\r\n");
		}
		delay_ms(100);
	}
}
