#include<reg52.h>
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
sbit k1=P1^6;  //�����������Ŷ��� 
sbit k2=P1^7;  //�����������Ŷ��� 
sbit Led1=P1^4;
sbit Led2=P1^5;

sbit rest=P2^7; //����оƬ��λ���Ŷ���
sbit  dat=P2^6;  //����оƬ���������Ŷ���
sbit busy=P2^5;  //����оƬ---���Ŷ���
uchar a,b=1;
void music(uchar i);
void  delay(uint i);

void main()
	{
	uchar i=1;
	    Led1=0;
		Led2=1;
	    P0=0;
	
	delay(100);	
	while (1)
		{
		   	delay(5000); 
			music(34); 
			while (!busy);
			delay(5000); 	
			Led1=~Led1;
			delay(5000); 
			Led2=~Led2;	
		}
	}
 



void  delay(uint i)
	{
	while(i--);
	}
                                        

                                                              //���м���Լ�������������     



void music(uchar i)                             //����music������������
	{	
	dat=0;  
	rest=0;
	delay(50);
	rest=1;					
	delay(50);
	rest=0;
    delay(50);
	
	dat=0;
	delay(50);
	
	for(a=0;a<i;a++)
       {
		dat=1;
		delay(50);
		dat=0;
		delay(50);   
	   }
	}