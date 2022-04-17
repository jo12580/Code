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
//	uint8_t  val = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);//1 ��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ���͵�ƽʱ����	
//	if(val==0)			//�������� ��¼���µ�ʱ��
//	{
//		count++;
//	}
//	if(val==1)					//�����ɿ� ����ֵ���㲢���ذ��µ�ʱ��ֵ
//	{
//		if(count>100)//���¾���1s  10*10ms=100ms
//		{
//			KEY_flag = 2;			//����
//			HAL_UART_Transmit(&huart2,data1,20,100);
//		}
//		else if(count>10 && count <100)//��ʾ����10ms
//		{
//			KEY_flag = 1;			//�̰�
//			HAL_UART_Transmit(&huart2,data2,20,100);
//		}
////		else
////		{
////			//����ʱ��
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
//	ÿ10ms����һ�θú��������һ������״̬
//	�����ö�ʱ����Ҳ�����ò�ѯ
//*/
//unsigned int KEY_flag=0;
//uint16_t count=0;
//uint8_t data1[20]="key_flag == long\r\n";
//uint8_t data2[20]="key_flag == short\r\n";

//void Key_scan(void)
//{
//	//1 ��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ���͵�ƽʱ����	
//	uint8_t  val = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);
//	if(val==0)//�������� ��¼���µ�ʱ��
//	{
//		count++;
//	}
//	else //�����ɿ� ����ֵ���㲢���ذ��µ�ʱ��ֵ
//	{
//		if(count>100)//���¾���1s  10*10ms=100ms
//		{
//    		//����
//			KEY_flag = 2;
////			printf("key_flag == long\r\n");
//			HAL_UART_Transmit(&huart2,data1,20,100);
//		}
//		else if(count>10 && count <100)//��ʾ����10ms
//		{
//				//�̰�
//			KEY_flag = 1;
////			printf("key_flag == short\r\n");
//			HAL_UART_Transmit(&huart2,data2,20,100);
//		}
//		else
//		{
//			//����ʱ��
//		}
//		count=0;
//	}
//}

///*
//*���ݱ�־λ�ı�LED��״̬
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

