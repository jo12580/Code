#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "lcd.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

u8 Disp[]="�Ұ����ӣ��Ұ�ʦ��";
u8 aaa[]="ŷ����� 201912700030" ;

void main(void)
{

   if(k2==0)
{	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp[i]);	
	}
	while(1);	
}	
	
	
	if(k1==0)	
{
	   	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
		{
		LcdWriteData(aaa[i]);	
		}
	while(1);
}
		
}
