#include<reg52.h>
#include <IIC-1602.h>
//#include"infrared.h"
//#include"delay.h"

void  delay(unsigned int i);
sbit LED1=P3^4;  //��Ҫ�����Գ�����
sbit IRIN=P3^2;

unsigned char IrValue[6];
unsigned char  Time;
code unsigned cmd_infrareed[]={0x19,0x45,0x46,0x47,0x44,0x40,0x43,0x07,0x15,0x09,0x16,0x0b,0x18,0x08,0x1c,0x5a,0x52}; // 0~9   A~G

void IrInit()  // �ⲿ�ж�0��ʼ��
{
	IT0=1;//�½��ش���
	EX0=1;//���ж�0����
	EA=1;	//�����ж�
	IRIN=1;//��ʼ���˿�
}

void main()
{  
	IrInit();  // �ⲿ�ж�0��ʼ��
	Lcd1602Iinitial();
	DispOneChar(0,0,'1');        //��ʾһ���ַ�
  disp_string(0,0,"123456");       //��ʾһ���ַ���
	delay(40000);
  disp_string(0,0,"      ");       //��ʾ����������Ŀո��൱������
while(1)
{
	   	
		switch(IrValue[2])
	 		 {
				 case(0x19): disp_string(0,1," 0");  //����0
							  break;					  
		     case(0x45): disp_string(0,1," 1");  //����1
							  break;					 
				 case(0x46): disp_string(0,1," 2");  //����2 
							  break;					  
				 case(0x47): disp_string(0,1," 3");  //����3
							  break;					  
				 case(0x44):  disp_string(0,1," 4");  //����4
							  break;					 
				 case(0x40):  disp_string(0,1," 5");  //����5
							  break;					  
				 case(0x43):  disp_string(0,1," 6");  //����6
							  break;					  
				 case(0x07):  disp_string(0,1," 7");  //����7
							  break;					  
				 case(0x15):  disp_string(0,1," 8");  //����8
							  break;					  
				 case(0x09):  disp_string(0,1," 9");  //����9
							  break;					  
				 case(0x16):  disp_string(0,1," *");  //����*
							  break;					  
				 case(0x0d):  disp_string(0,1," #");  //����#
							  break;					  
				 case(0x18):  disp_string(0,1," ^");  //����^
							  break;					  
				 case(0x08):  disp_string(0,1," <");  //���� <
							  break;					  
		         case(0x1c):disp_string(0,1,"ok");  //����ok
							  break;					  
				 case(0x5a):  disp_string(0,1," >");  //����>
							  break;					  
				 case(0x52):  disp_string(0,1," V");  //����V
							  break;					  
	}  
	  DispOneChar(4,1,IrValue[2]/16+0X30);	//��ʾ������  ΪʲôҪ����0x30
	  
	  if( IrValue[2]%16<10 )   // С��10��������0-9,��������ĸ��abcdefg
      {DispOneChar(5,1,IrValue[2]%16+0X30);}  //��ʾʮλֵ  ΪʲôҪ����0x30
		else
			{DispOneChar(5,1,IrValue[2]%16+0X37);}  //��ʾ��λֵ  ΪʲôҪ����0x37
			
		LED1= ~LED1;
		delay(40000);

	
}}

void ReadIr() interrupt 0   //�ⲿ�ж���������
{
	unsigned int  j,k;
	unsigned char  err;
	Time=0;					 
	delay(700);	//7ms
	if(IRIN==0)		//ȷ���Ƿ���Ľ��յ���ȷ���ź�
	{	 
		
		err=1000;				//1000*10us=10ms,����˵�����յ�������ź�
		/*������������Ϊ����ѭ���������һ������Ϊ�ٵ�ʱ������ѭ������ó�������ʱ
		�������������*/	
		while((IRIN==0)&&(err>0))	//�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
		{			
			delay(1);
			err--;
		} 
		if(IRIN==1)			//�����ȷ�ȵ�9ms�͵�ƽ
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
			{
				delay(1);
				err--;
			}
			for(k=0;k<4;k++)		//����4������
			{				
				for(j=0;j<8;j++)	//����һ������
				{

					err=60;		
					while((IRIN==0)&&(err>0))//�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
					{
						delay(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //����ߵ�ƽ��ʱ�䳤�ȡ�
					{
						delay(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k��ʾ�ڼ�������
					if(Time>=8)			//����ߵ�ƽ���ִ���565us����ô��1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//����ʱ��Ҫ���¸�ֵ							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}
 
//---------��ʱ����-------------
void  delay(unsigned int i)
{
	while(i--);
}
