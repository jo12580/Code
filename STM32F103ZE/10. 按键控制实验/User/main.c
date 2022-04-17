

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"


int main()
{
	u8 key,i;
	SysTick_Init(72);
	LED_Init();
	KEY_Init();
	
	while(1)
	{
		key=KEY_Scan(0);   //ɨ�谴��
		switch(key)
		{
			case KEY_UP: led2=0;break;      //����K_UP����    ����D2ָʾ��
			case KEY_DOWN: led2=1;break;    //����K_DOWN����  Ϩ��D2ָʾ��
			case KEY_LEFT: led3=1;break;    //����K_LEFT����  ����D3ָʾ��
			case KEY_RIGHT: led3=0;break;   //����K_RIGHT���� Ϩ��D3ָʾ��
		}
		i++;
//		if(i%20==0)
//		{
//			led1=!led1;      //LED1״̬ȡ��
//		}
		delay_ms(10);	
	}
}
