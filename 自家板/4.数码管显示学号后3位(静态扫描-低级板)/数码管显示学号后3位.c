#include<reg52.h>
typedef unsigned int u16;	 
typedef unsigned char u8;
sbit a1=P0^1; 
sbit a2=P0^2;
sbit a3=P0^3;
sbit a4=P0^4;
sbit a5=P0^5;
sbit a6=P0^6;
sbit a7=P0^7;
sbit a0=P0^0;

void delay(u16 i)
{
	while(i--);	
}

void main()
{
      while(1)
	  {
	  delay(50000);
a1=1 ;		               //0
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;
      delay(50000);
delay(50000);               //3
a1=0 ;
a2=1 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=1 ;
	  delay(50000);
a1=1 ;		               //0
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;

	  }
}