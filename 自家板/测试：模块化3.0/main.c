#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "ds1302.h"	
#include "key.h"



void main()
{	

	Ds1302Init();	   //��һ�γ�ʼ����Ϳ���ע�͸�����䣬�����´������Ͳ����ٴγ�ʼ����
			        key();	
	while(1)
	{
		datapros();	 //���ݴ�����
		DigDisplay();//�������ʾ����

	}		
}






