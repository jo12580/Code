#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"


/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
int main()
{
	u8 i=0;
	u16 value=0;
	float vol;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	ADCx_Init();
	
	while(1)
	{
//		i++;
//		if(i%20==0)
//		{
//			led1=!led1;
//		}
//		
//		if(i%50==0)
//		{
			value=Get_ADC_Value(ADC_Channel_1,20);
			printf("���ADֵΪ��%d\r\n",value);
			vol=(float)value*(3.3/4096);
			printf("����ѹֵΪ��%.2fV\r\n",vol);
//		}
		delay_ms(10);	
	}
}
