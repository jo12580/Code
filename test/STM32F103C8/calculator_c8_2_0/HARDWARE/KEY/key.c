#include "key.h"
#include "delay.h"


#define row 0
#define col 1					
#define RCC_APB2_GPIOX  RCC_APB2Periph_GPIOB//IOʱ��					
#define MAX_Time   36000000 // ���ּ�������ʱ
#define MAX_row 5
#define MAX_col 4
#define MAX_Number (MAX_row > MAX_col)?MAX_row:MAX_col

typedef struct 
{
	GPIO_TypeDef* GPIOX;
	uint16_t GPIO_Pin;
}KGPT[2][MAX_Number]; //5*5 ����   ʵ����  5 * 4

KGPT KPIN={ //ֻ��Ҫ��������д�����е�IO�ں͹ܽ�   IO�ں͹ܽŶ��������ⶨ�� ������һ��IO�� �ܽ�Ҳ���谴˳��
						{{GPIOB,GPIO_Pin_5},{GPIOB,GPIO_Pin_6} ,{GPIOB,GPIO_Pin_7}, {GPIOB,GPIO_Pin_8},{GPIOB,GPIO_Pin_9}},//row ����
						{{GPIOA,GPIO_Pin_1},{GPIOA,GPIO_Pin_2}, {GPIOA,GPIO_Pin_3},{GPIOA,GPIO_Pin_4}}  //col ����
					};

					
//������ʼ������ 
void KEY_Init(void)
{
  u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2_GPIOX,ENABLE);//ʹ��PORTCʱ��
  for(i=0;i<MAX_row;i++)
	{
	 GPIO_InitStructure.GPIO_Pin = KPIN[row][i].GPIO_Pin;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
	 GPIO_Init(KPIN[row][i].GPIOX, &GPIO_InitStructure);//��ʼ��row
	 GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);//ÿ��Ϊ00000
	}
 for(i=0;i<MAX_col;i++)
	{
		 GPIO_InitStructure.GPIO_Pin  = KPIN[col][i].GPIO_Pin;//PC5
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
		 GPIO_Init(KPIN[col][i].GPIOX, &GPIO_InitStructure);//��ʼ��col
	}
} 
//�м��
u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex)
{
 u8 i;
 DelayMs(10);//������ʱ
 for(i = 0;i<MAX_row;i++){
  GPIO_SetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);//ÿ������1
  if((GPIO_ReadInputDataBit(GPIOX, colPin) == 1)) //�����Ҳ���� �е�ֵ��֪���� Ϊ i
		{
			GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin); //�лָ� ��0 
		  return colIndex+i*4;//���ص����� Ϊ0-19 ��Ӧ5x4���̵�20����
		}
	GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);     //�лָ� ��0
 }
 return 0;
}
//�м��
u8 KEY_Scan(void)
{	
 u8 i,keyvalue=21;
	//u32 j=0;
	for(i = 0;i<MAX_col;i++)
	{
	 if(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)//����� ��ֵΪ i
	 {
			keyvalue = ReScan(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin,i);//����� ȡ��ֵ
		  while(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0);
			//while((GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0))j++;//���ּ��
			return keyvalue;//���ؼ�ֵ
		}
	}
return 21;
}

