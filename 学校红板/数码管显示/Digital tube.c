#include"Digital tube.h"

 u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

void DigDisplay()
{
	unsigned char i;
	unsigned int j;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(2):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(3):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(4):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ 
			case(5):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ 
			case(6):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ 
			case(7):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ	
		}
		P0=smgduan[i];//���Ͷ���
		j=10;						 //ɨ����ʱ���趨
		while(j--);	
	P0=0x00;//����
	}
}