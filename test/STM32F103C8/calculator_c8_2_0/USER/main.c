#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "delay.h"
#include "time.h"
#include "oled.h"
#include "gui.h"
#include "iic.h"
#include "stdlib.h"
#include "calculator.h"
#include "string.h"

/*
   AA 55  ֡ͷ    55 AA ֡β
	 ����λ16����   
									1 -- 1
									2 -- 2
									3 -- 3
									4 -- 4
									5 -- 5
									6 -- 6
									7 -- 7
									8 -- 8
									9 -- 9
								 10 -- ��
								 11 -- ��
								 12 -- ��
								 13 -- ��
								 14 -- ��
								 15 -- �η�
								 16 -- ����
								 17 -- ����
								 18 -- ���
								 19 -- ��

*/

//���ڷ��ؾ�����̵İ�����ֵ
unsigned char temp = 21;
USART_TypeDef * DEBUG_USARTx = USART1;
//�����жϽ��յ�����
char Rxbuff[128]="";
unsigned char Rxbuff_cnt = 0;

//���ڼ�¼�ػ�ʱ��ı���
unsigned char Shutdown = 0;
//�Ƿ񼤻���Ļ�ı�־
unsigned char Activity_flag = 1;
//Activity_flag = 1ʱΪ ����״̬  Activity_flag = 0Ϊ��Ҫ�ػ�״̬ 
unsigned char Shutdown_flag = 0;//0��ʾû�ػ���1��ʾ�ػ�
//�������
void calculator_voice(char *expression);

extern unsigned char Rxbuff_cnt;
extern char Rxbuff[128];
extern USART_TypeDef * DEBUG_USARTx;
extern unsigned char temp;
extern unsigned char Shutdown;
extern unsigned char Activity_flag;
extern unsigned char Shutdown_flag;

int main(void)
{
	char TxBuff[5]= {0XAA,0X55,0X00,0X55,0XAA};
	//���ڴ����������ʽ
	char expression[128] = "#";
	unsigned char expression_wei = 1;//��¼��ʽ��λ��  ��0λ�̶�Ϊ #
	unsigned char expression_display = 1;//��¼��ʾ��λ��
	unsigned char expression_display_line = 0;//��ʾ��ʾ�� Y
	unsigned char Symboling_flag = 0;//��ʾ��ǰ����������������
	unsigned char calculator_complete_flag = 0;//��ʾ�ϴμ����Ƿ����
	unsigned char calculator_old = 0;//�����Ƿ����ϵ�����
	char result_display[20];//������ʾ���
	char result_display_old[20];//���ڴ��ϴμ�����
	double result = 0.0;//������
	unsigned char i = 0;//ѭ������
	//�����ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 //��ʱ������ʼ��
	uart_init(115200);//���ڳ�ʼ��
	
	KEY_Init();
	//oled��ʼ��
	
	OLED_Init();			         //��ʼ��OLED  
	OLED_Clear(0);             //������ȫ�ڣ�
	TIM3_Int_Init(999,7199);//��ʱ����ʼ��  100ms ��Ƶ72M
	uart3_init(9600);//��������ʼ��
	
	DEBUG_USARTx = USART3;
	
	while(1)
	{
		//������ʾ�����¼
		if(Shutdown_flag == 1 && Activity_flag == 1)
		{
			Shutdown_flag = 0;
			strcpy(result_display_old,result_display);//���ƽ��
			GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
		}
		//�����ж�
		if(Shutdown == 100)
		{
			OLED_Clear(0);
			expression_wei = 1;expression_display = 1;expression_display_line = 0;Symboling_flag = 1;
					for(i = 1; i < 128; i++)
						expression[i] = '\0';
			Shutdown = 0;
			Shutdown_flag = 1;
		}
		//�ж�expression_display�Ƿ���� > 16 
		if(expression_display >= 16)
		{
			expression_display = 1;
			expression_display_line++;
		}
		//���ܴ���ͨ�ŵ�����
		if(Rxbuff_cnt > 0)
		{
			Activity_flag = 1;
			GUI_ShowString(expression_display * 8,8 * expression_display_line,Rxbuff,8,1);
			//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
			
			if(strcmp("=",Rxbuff) == 0)
			{
				//����#��\0
				expression[expression_wei] = '#';
				expression_wei++;
				expression[expression_wei] = '\0';
				expression_display++;
				strcpy(result_display_old,result_display);//���ƽ��
				result  = calculator(expression);
				sprintf(result_display,"%f",result);
				GUI_ShowString(expression_display*8,8 * expression_display_line,result_display,8,1);//��ʾ������
				GUI_FillRectangle(0, 56, 128, 64,0);
				GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
				calculator_voice(result_display);
				expression[1] = '\0';
				expression_wei = 1;expression_display = 1;expression_display_line = 0;Symboling_flag = 1;
				calculator_complete_flag = 1;//������
			}
			else if(strcmp("$",Rxbuff) == 0)
			{
				expression_wei = 1;expression_display = 1;expression_display_line = 0;Symboling_flag = 1;
				GUI_FillRectangle(0, 0, 128, 55,0); //����
					for(i = 1; i < 128; i++)
						expression[i] = '\0';
			}
			else
			{
				//�ַ���ƴ��
				strcat(expression,Rxbuff);
				expression_wei = expression_wei + Rxbuff_cnt;
				expression_display = expression_display + Rxbuff_cnt;
			}
			memset(Rxbuff,0,sizeof(Rxbuff));
			Rxbuff_cnt = 0;
		}
		
		switch(temp)
		{
			case 0:
				Activity_flag = 1;
					//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 1;
					expression[expression_wei] = '.';
						//OLED��ʾ
					GUI_ShowString(expression_display*8,8 * expression_display_line,".",8,1);
					expression_wei++;expression_display++;
				//����
				TxBuff[2] = 0X13;
				printf("%s",TxBuff);
				break;
			case 1:
				Activity_flag = 1;
					//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '0';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"0",8,1);
					expression_wei++;expression_display++;
				//����
				TxBuff[2] = 0X0A;
				printf("%s",TxBuff);
				break;
			case 2:
					Activity_flag = 1;
					expression[expression_wei] = '=';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"=",8,1);
					expression_wei++;expression_display++;
					//��� # ��'\0'Ȼ��ʼ����
					expression[expression_wei] = '#';
					expression[expression_wei+1] = '\0';
					Symboling_flag = 1;
					//��ʾ���
						//����
						TxBuff[2] = 0X11;
						printf("%s",TxBuff);
					//����
					result  = calculator(expression);
					strcpy(result_display_old,result_display);//���ƽ��
					sprintf(result_display,"%f",result);
					GUI_ShowString(expression_display*8,8 * expression_display_line,result_display,8,1);
					GUI_FillRectangle(0, 56, 128, 64,0);
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_voice(result_display);
					expression_wei = 1;expression_display = 1;expression_display_line = 0;
					calculator_complete_flag = 1;//������
	
				break;
			case 3:
				Activity_flag = 1;
					if(Symboling_flag == 0)
					{
						//��ʾ���ſ�ʼ����
						Symboling_flag = 1;
						expression[expression_wei] = '/';
						GUI_ShowString(expression_display*8,8 * expression_display_line,"/",8,1);
						expression_wei++;expression_display++;
						//����
				TxBuff[2] = 0X0E;
				printf("%s",TxBuff);
					}
				break;
			case 4:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '7';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"7",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X07;
				printf("%s",TxBuff);
				break;
			case 5:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '8';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"8",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X08;
				printf("%s",TxBuff);
				break;
			case 6:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '9';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"9",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X09;
				printf("%s",TxBuff);
				break;
			case 7:
				Activity_flag = 1;
					if(Symboling_flag == 0)
					{
						Symboling_flag = 1;
						expression[expression_wei] = '*';
						GUI_ShowString(expression_display*8,8 * expression_display_line,"*",8,1);
						expression_wei++;expression_display++;
							//����
				TxBuff[2] = 0X0D;
				printf("%s",TxBuff);
					}
				break;
			case 8:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '4';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"4",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X04;
				printf("%s",TxBuff);
				break;
			case 9:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '5';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"5",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X05;
				printf("%s",TxBuff);
				break;
			case 10:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '6';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"6",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X06;
				printf("%s",TxBuff);
				break;
			case 11:
			Activity_flag = 1;
					if(Symboling_flag == 0)
					{
						Symboling_flag = 1;
						expression[expression_wei] = '-';
						GUI_ShowString(expression_display*8,8 * expression_display_line,"-",8,1);
						expression_wei++;expression_display++;
						//����
				TxBuff[2] = 0X0C;
				printf("%s",TxBuff);
					}
				break;
			case 12:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '1';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"1",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X01;
				printf("%s",TxBuff);
				break;
			case 13:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '2';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"2",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X02;
				printf("%s",TxBuff);
				break;
			case 14:
				Activity_flag = 1;
				//�����ж��Ƿ�����ϴμ���
			if(calculator_complete_flag == 1)
			{
				calculator_complete_flag = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);
				if(calculator_old == 0)//��һ���ϴμ�¼
				{
					GUI_FillRectangle(0, 56, 128, 64,0);
					strcpy(result_display_old,result_display);//���ƽ��
					GUI_ShowString(8,56,result_display_old,8,1);//��ʾ�ϴν��
					calculator_old = 1;
				}
			}
					//��ʾ�����������
					Symboling_flag = 0;
					expression[expression_wei] = '3';
					GUI_ShowString(expression_display*8,8 * expression_display_line,"3",8,1);
					expression_wei++;expression_display++;
					//����
				TxBuff[2] = 0X03;
				printf("%s",TxBuff);
				break;
			case 15:
				Activity_flag = 1;
					if(Symboling_flag == 0)
					{
						Symboling_flag = 1;
						expression[expression_wei] = '+';
						GUI_ShowString(expression_display*8,8 * expression_display_line,"+",8,1);
						expression_wei++;expression_display++;
						//����
				TxBuff[2] = 0X0B;
				printf("%s",TxBuff);
					}
				break;
			case 16:
			Activity_flag = 1;
				//�˸��
				Symboling_flag = 0;
				expression[expression_wei - 1] = '\0';
				GUI_ShowString((expression_display - 1)*8,8 * expression_display_line," ",8,1);
				expression_wei = expression_wei - 1;expression_display = expression_display - 1;
				if(expression_wei < 0)
				{
					expression_wei = 0;
				}
				if(expression_display < 1)
				{
					expression_display = 1;
				}
				break;
			case 17://��0
		Activity_flag = 1;
					expression_wei = 1;expression_display = 1;Symboling_flag = 1;expression_display_line = 0;
				GUI_FillRectangle(0, 0, 128, 55,0);//�����ʾ�����������Ļ
					//����
				TxBuff[2] = 0X012;
				printf("%s",TxBuff);
					for(i = 1; i < 128; i++)
						expression[i] = '\0';
				break;
			case 18:
		Activity_flag = 1;
					if(Symboling_flag == 0 || expression_wei == 1 || !(expression[expression_wei - 1] >= '0' && expression[expression_wei - 1] <='9'))
					{
						if(expression_wei == 1)
						{
							Symboling_flag = 1; 
							expression[expression_wei] = 'r';
							GUI_ShowString(expression_display*8,8 * expression_display_line,"r",8,1);
							expression_wei++;expression_display++;
						}
						else if(!(expression[expression_wei - 1] >= '0' && expression[expression_wei - 1] <='9'))
						{
							Symboling_flag = 1;
							expression[expression_wei] = 'r';
							GUI_ShowString(expression_display*8,8 * expression_display_line,"r",8,1);
							expression_wei++;expression_display++;
						}
						else
						{
							Symboling_flag = 1;
							expression[expression_wei] = '*';
							GUI_ShowString(expression_display*8,8 * expression_display_line,"*",8,1);
							expression_wei++;expression_display++;
							expression[expression_wei] = 'r';
							GUI_ShowString(expression_display*8,8 * expression_display_line,"r",8,1);
							expression_wei++;expression_display++;
						}
						//����
						TxBuff[2] = 0X10;
						printf("%s",TxBuff);
					}
				break;
			case 19:
		Activity_flag = 1;
					if(Symboling_flag == 0)
					{
						Symboling_flag = 1;
						expression[expression_wei] = '^';
						GUI_ShowString(expression_display*8,8 * expression_display_line,"^",8,1);
						expression_wei++;expression_display++;
						//����
				TxBuff[2] = 0X0F;
				printf("%s",TxBuff);
					}
				break;
			default:
				break;
		}
		
	}
	
}


void calculator_voice(char *expression)
{
	//���ݲ�ͬ���ַ����Ͳ�ͬ������
	char TxBuff[5] = {0XAA,0X55,0X00,0X55,0XAA};
	char Voice_Flag[19] = {0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,0X10,0X11,0X12,0X13};
	unsigned char i = 0;
	while(expression[i] != '\0')
	{
		DelayMs(500);//ȷ���������
		switch(expression[i])
		{
			case '0':
					TxBuff[2] = Voice_Flag[9];printf("%s",TxBuff);
						break;
			case '1':
					TxBuff[2] = Voice_Flag[0];printf("%s",TxBuff);
						break;
			case '2':
					TxBuff[2] = Voice_Flag[1];printf("%s",TxBuff);
						break;
			case '3':
					TxBuff[2] = Voice_Flag[2];printf("%s",TxBuff);
						break;
			case '4':
					TxBuff[2] = Voice_Flag[3];printf("%s",TxBuff);
						break;
			case '5':
					TxBuff[2] = Voice_Flag[4];printf("%s",TxBuff);
						break;
			case '6':
					TxBuff[2] = Voice_Flag[5];printf("%s",TxBuff);
						break;
			case '7':
					TxBuff[2] = Voice_Flag[6];printf("%s",TxBuff);
						break;
			case '8':
					TxBuff[2] = Voice_Flag[7];printf("%s",TxBuff);
						break;
			case '9':
					TxBuff[2] = Voice_Flag[8];printf("%s",TxBuff);
						break;
			case '.':
					TxBuff[2] = Voice_Flag[18];printf("%s",TxBuff);
						break;
			default:
				break;

		}
		i++;
		
	}
}


