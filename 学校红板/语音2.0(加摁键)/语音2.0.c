	#include<reg52.h>
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
sbit k1=P1^6;  //�����������Ŷ��� 
sbit k2=P1^7;  //�����������Ŷ��� 
sbit k3=P1^0;  //�����������Ŷ��� 
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
			
	    Led1=0;
		Led2=1;
	    P0=0;
	
	delay(100);	
	while (1)  {

	         if(k1==0)
{music(26);
while(!busy);		 //����
music(36) ;	  //��ӭ�ٴι���
while(!busy) ;
music(37)  ;}  //����ʦ����ѧ


             if(k2==0)
{music(26);
while(!busy);		 //����
music(36);	  //��ӭ�ٴι���
  while(!busy) ;
music(38)  ;  //���ӹ���ѧԺ
  while(!busy)	;
music(46) ;	}	//��ѧ�����»���


             if(k3==0)
{music(26);
while(!busy);		 //����
music(28);		 //������
  while(!busy) ;
music(4);		 //2
  while(!busy) ;
music(2);		 //0
  while(!busy);
music(3);		 //	1
while(!busy)  ;
music(11);		 //	9
while(!busy);
music(17);		 //��
while(!busy);
music(12);		 //ʮ
while(!busy) ;
music(3);		 //һ
while(!busy) ;
music(18);		 //��
while(!busy) ;
music(4);		 //��
while(!busy) ;
music(12);		 //ʮ 
while(!busy) ;
music(19);		 //��
while(!busy) ;
music(34);		 //��ӭ���ٱ���
while(!busy) ;
music(35);	}	 //��ӭʹ�ñ���˾��Ʒ

   }    
}
 

void  delay(uint i)
{
	while(i--);
}

void music(uchar i)								//��������������
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
		