#include<reg52.h>
typedef unsigned int u16;	 
typedef unsigned char u8;
sbit a1=P2^0;
sbit a2=P2^1;
sbit a3=P2^2;
sbit a4=P2^3;
sbit a5=P2^4;
sbit a6=P2^5;
sbit a7=P2^6;
sbit a8=P2^7;

void delay(u16 i)
{
	while(i--);	
}

void main()
{
   			while(1000)
  		{	
		delay(5000);
		a1=0;
		delay(5000);
		a2=0;
		delay(5000);
		a3=0;
		delay(5000);
		a4=0;
		delay(5000);
		a5=0;
		delay(5000);
		a6=0;
		delay(5000);
		a7=0;
		delay(5000);
		a8=0;
		        delay(5000);
		a8=1;
		        delay(5000);
		a7=1;
		        delay(5000);
		a6=1;
				delay(5000);
		a5=1;
				delay(5000);
		a4=1;	
				delay(5000);
		a3=1;	
				delay(5000);
		a2=1;	
				delay(5000);
		a1=1;		
		}        
}