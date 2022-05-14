#include "main_task.h"
#include "stdio.h"
#include "oled.h"
#include "ds18b20.h"

#define time_sleep 20000		//��������ʱ�䣬��λms
#define time_alarm 10000		//��������ʱ�䣬��ms
char lcdLine_1st_line[16];		//��ʾ����ʾ���ݵĺ���
char lcdLine_2st_line[16];
unsigned int time_100ms=1,time_500ms=1,sleep_time=1,alarm_time=1;
unsigned int key_flag=0,alarm_flag=0;		//��־λ
unsigned int key_1=0;	//��¼����1���´���
float value;

void key(void);
void sleep(void);
void time(void);
void alarm(void);

void main_init()
{
	HAL_Delay(100);
	OLED_Init();
	HAL_TIM_Base_Start_IT(&htim1);
	sprintf(lcdLine_2st_line, "welcome");
	DS18B20_Init();
}

void main_task()
{
	if(time_500ms==1)
	{
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);		//��ȡʱ��
		HAL_RTC_GetDate(&hrtc, &sdata, RTC_FORMAT_BIN);		//��ȡ����
		value=DS18B20_Get_Temp();		//�¶Ȳ���
		time_500ms=0;
	}
	if(time_100ms==1)
	{
		OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
		OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
		time_100ms=0;
	}
	key();
	time();
	alarm();
} 

void key()		//������ʾ���ڶ���
{
	static unsigned int hour=0,min=0,sec=0;
	static unsigned int h=0,m=0,s=0;
	static unsigned int time_allow=0,clock_allow=0,temp_allow=0;
	static unsigned int alarm_status=0;
	if(key_flag!=0)
	{
		sleep_time=1;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
		if(key_flag==1)		//����ʱ�䡢���ӡ��¶Ȳ鿴�İ���
		{
			if(key_1==0)		//�鿴ʱ��
			{
				OLED_Clear();
				hour=sTime.Hours;min=sTime.Minutes;sec=sTime.Seconds;
				time_allow=1;clock_allow=0;temp_allow=0;
				OLED_ShowString(84, 2, (unsigned char *)"time", 8);
			}
			if(key_1==1)		//�鿴����
			{
				OLED_Clear();
				h=0;m=0;s=0;
				time_allow=0;clock_allow=1;temp_allow=0;
				OLED_ShowString(84, 2, (unsigned char *)"alarm ", 8);
			}
			if(key_1==2)		//�鿴�¶�
			{
				OLED_Clear();
				time_allow=0;clock_allow=0;temp_allow=1;
				OLED_ShowString(45, 2, (unsigned char *)".C", 8);
			}
			key_1++;
			if(key_1==3)	key_1=0;
			key_flag=0;
		}
		if(key_flag==2)		//ȷ�ϼ�
		{
			OLED_Clear();
			if(time_allow==1)
			{
				sTime.Hours=hour;
				sTime.Minutes=min;
				sTime.Seconds=sec;
				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			}
			if(clock_allow==1)
			{
				sAlarm.AlarmTime.Hours = h;
				sAlarm.AlarmTime.Minutes = m;
				sAlarm.AlarmTime.Seconds = s;
				HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
				alarm_status=1;
			}
			key_flag=0;
		}
//		if(key_flag==6)		//�ϵ��
//		{
//			time_allow=0;clock_allow=0;
//			sleep_time=time_sleep-1;	//��һ��1ms��ϵ�
//			key_flag=0;
//		}
	}
	//���´��룬��������1֮��Ĳ���
	{
		if(time_allow==1)		//ʱ�����ý�����
		{
			if(key_flag!=0)
			{
				switch (key_flag)
				{
					case 3:
					{
						hour++;
						if(hour==24)	hour=0;
					}break;
					case 4:
					{
						min++;
						if(min==60)		min=0;
					}break;
					case 5:
					{
						sec++;
						if(sec==60)		sec=0;
					}break;
				}
			}
			if(sleep_time!=0)
				sprintf(lcdLine_2st_line, "%2d:%2d:%2d",hour,min,sec);
			key_flag=0;
		}
		if(clock_allow==1)		//�������ý�����
		{
			if(key_flag!=0)
			{
				switch (key_flag)
				{
					case 3:
					{
						h++;
						if(h==24)	h=0;
					}break;
					case 4:
					{
						m++;
						if(m==60)		m=0;
					}break;
					case 5:
					{
						s++;
						if(s==60)		s=0;
					}break;
					case 6:
					{
						HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_A);
						alarm_status=0;
					}break;
				}
			}
			if(sleep_time!=0)
			{
				if(alarm_status==1)
				{
					OLED_ShowString(84, 2, (unsigned char *)"alarm ", 8);
					OLED_ShowString(84, 3, (unsigned char *)"ON", 8);
				}
				else
				{
					OLED_ShowString(84, 2, (unsigned char *)"alarm ", 8);
					OLED_ShowString(84, 3, (unsigned char *)"OFF", 8);
				}
				sprintf(lcdLine_2st_line, "%2d:%2d:%2d",h,m,s);
			}
				
			key_flag=0;
		}
		if(temp_allow==1)		//�¶Ƚ�����
		{
			if(sleep_time!=0)
				sprintf(lcdLine_2st_line, "%.2f",value);
			key_flag=0;
		}
		
	}
}
void time()		//������ʾ����һ�С���ʱ��
{
	if(sleep_time!=0)
	{
		if(sTime.Hours<13)
		{
			sprintf(lcdLine_1st_line, "%2d-%2d-%2d    AM",sTime.Hours,sTime.Minutes,sTime.Seconds);
		}
		if(sTime.Hours>13)
		{
			sprintf(lcdLine_1st_line, "%2d-%2d-%2d    PM",sTime.Hours,sTime.Minutes,sTime.Seconds);
		}
	}
}
void alarm()		//�����¼�
{
	if(alarm_flag==1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		if(key_flag!=0)			//�а������£�����ͣ
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			alarm_flag=0;
		}
		if(alarm_time==0)		//����ʱ�䵽
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			alarm_flag=0;
		}
	}
}
void sleep()		//����
{
	if(sleep_time==0)
	{
		key_1=0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		OLED_Clear();	
		sprintf(lcdLine_1st_line, "                        ");
		sprintf(lcdLine_2st_line, "                        ");
	}
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	alarm_flag=1;
	alarm_time=1;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static unsigned int i=0;
	if(GPIO_Pin==GPIO_PIN_5)
	{
		i++;
		if(i==2)
		{
			key_flag=1;		//���ü�
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_6)
	{
		i++;
		if(i==2)
		{
			key_flag=2;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_7)
	{
		i++;
		if(i==3)
		{
			key_flag=3;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_8)
	{
		i++;
		if(i==2)
		{
			key_flag=4;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_9)
	{
		i++;
		if(i==2)
		{
			key_flag=5;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_10)
	{
		i++;
		if(i==2)
		{
			key_flag=6;
//			HAL_NVIC_SystemReset();
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_11)
	{
		i++;
		if(i==2)
		{
			key_flag=7;
			i=0;
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		static unsigned int a1,b;
		a1++;b++;
		if(a1==100)
		{
			time_100ms=!time_100ms;
			a1=0;
		}
		if(b==500)
		{
			time_500ms=!time_500ms;
			b=0;
		}
		if(alarm_time!=0)
		{
			alarm_time++;
			if(alarm_time>time_alarm)
			{
				alarm_time=0;
			}
		}
		if(sleep_time!=0)
		{
			sleep_time++;
			if(sleep_time>time_sleep)
			{
				sleep_time=0;
				sleep();
			}
		}
	}
}
