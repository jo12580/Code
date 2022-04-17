#include <reg52.h>			
typedef unsigned int u16;	  
typedef unsigned char u8;
sbit beep=P2^5;

typedef unsigned char uchar	  ;	    //�ӳٶ���
typedef unsigned int  uint	;
void  delay(uint i);
								  
sbit LSA=P2^2;					    //�����
sbit LSB=P2^3;
sbit LSC=P2^4;
u8 code sz0_7[8]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};//��ʾ0~7��ֵ
u8 code	xh031[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x4f,0x06};//��ʾ00000031
								    
sbit k1=P3^1;                       //����
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;

						void LED1()
{
	u8 i;
		 while (1)
	{for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
		   case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//1
			case(2):
				LSA=1;LSB=0;LSC=1; break;//2
			case(3):	
				LSA=0;LSB=0;LSC=1; break;//3
			case(4):
				LSA=1;LSB=1;LSC=0; break;//4
			case(5):
				LSA=0;LSB=1;LSC=0; break;//5
			case(6):
				LSA=1;LSB=0;LSC=0; break;//6
			case(7):
				LSA=0;LSB=0;LSC=0; break;//7	
		}
		P0=sz0_7[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}
}
						void LED2()
{
	u8 i;
		 while (1)
		 {
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
		   case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//1
			case(2):
				LSA=1;LSB=0;LSC=1; break;//2
			case(3):	
				LSA=0;LSB=0;LSC=1; break;//3
			case(4):
				LSA=1;LSB=1;LSC=0; break;//4
			case(5):
				LSA=0;LSB=1;LSC=0; break;//5
			case(6):
				LSA=1;LSB=0;LSC=0; break;//6
			case(7):
				LSA=0;LSB=0;LSC=0; break;//7	
		}
		P0=xh031[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
	  }
}


                                  void delay(u16 i)
{
	while(i--);	
}

void abc()
		{	   while(1)
		{
		beep=~beep;
		delay(1000); //��ʱ��Լ1ms   ͨ���޸Ĵ���ʱʱ��ﵽ��ͬ�ķ���Ч��	
		}
		}

 void main()
{
			if(k1==0)
	{	
	if(k1==0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
		LED1();													    //�������ʾ����	
		}
		while(!k1);	 //��ⰴ���Ƿ��ɿ�
	}                                                   
	}

					
			if(k2==0)
	{		  
		delay(1000);   //�������� һ���Լ10ms
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			   abc();
		}
		while(!k2);	 //��ⰴ���Ƿ��ɿ�
	
	}

			if(k3==0)
		{
				delay(1000);   //�������� һ���Լ10ms
		if(k3==0)	 //�ٴ��жϰ����Ƿ���
		{
	beep=1;
		}
		while(!k3);	 //��ⰴ���Ƿ��ɿ�
		} 
		



			if(k4==0)
	{	
         delay(1000);   //�������� һ���Լ10ms
		if(k4==0)	 //�ٴ��жϰ����Ƿ���
		{
      LED2();
		}
		while(!k4);	 //��ⰴ���Ƿ��ɿ�
	}

}





