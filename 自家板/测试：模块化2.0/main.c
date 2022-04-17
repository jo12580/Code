#include <reg52.h>
#include "key.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include"ds1302.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

#define GPIO_DIG P0

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

char num=0;
u8 DisplayData[8];
u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ


/*******************************************************************************
* �� �� ��         : datapros()
* ��������		   : ʱ���ȡ����ת������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void datapros() 	 
{
   	Ds1302ReadTime();
	DisplayData[0] = smgduan[TIME[2]/16];				//ʱ
	DisplayData[1] = smgduan[TIME[2]&0x0f];				 
	DisplayData[2] = 0x40;
	DisplayData[3] = smgduan[TIME[1]/16];				//��
	DisplayData[4] = smgduan[TIME[1]&0x0f];	
	DisplayData[5] = 0x40;
	DisplayData[6] = smgduan[TIME[0]/16];				//��
	DisplayData[7] = smgduan[TIME[0]&0x0f];
}


/*******************************************************************************
* ������         :DigDisplay()
* ��������		 :�������ʾ����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<7;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��1λ
			case(2):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��2λ
			case(3):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��3λ
			case(4):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��4λ
			case(5):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��5λ
			case(6):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��6λ
		}
		P0=DisplayData[i];//��������
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}		
}




void main()
{	
	LSA=1; //��һ��������ṩλѡ
	LSB=1;
	LSC=1;

	while(1)
	{
		u8 k;
		k=KeyDown();			   
		GPIO_DIG=smgduan[k];	  
	}		
}

