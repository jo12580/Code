#include "reg52.h"			
typedef unsigned int u16;	 
typedef unsigned char u8;
sbit a1=P0^1; 											    // ˝¬Îπ‹Œª÷√1234  a1 a2 a3 a4
sbit a2=P0^2;													      //5678  a5 a6 a7 a0
sbit a3=P0^3;
sbit a4=P0^4;
sbit a5=P0^5;
sbit a6=P0^6;
sbit a7=P0^7;
sbit a0=P0^0;

sbit led=P2^0;		

void delay(int i)
{
	while(i--);	
}

void main()
{
			while(1)
	{ delay(50000);//0
a1=1 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;

delay(50000);//1
a1=1 ;
a2=1 ;
a3=1 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;

delay(50000);//2
a1=0 ;
a2=2 ;
a3=0 ;
a4=0 ;
a5=1 ;
a6=0 ;
a7=0 ;

delay(50000);//3
a1=0 ;
a2=1 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=1 ;

delay(50000);//4
a1=0 ;
a2=0 ;
a3=1 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;

delay(50000);//5
a1=0 ;
a2=0 ;
a3=0 ;
a4=1 ;
a5=0 ;
a6=0 ;
a7=1 ;

delay(50000);//6
a1=0 ;
a2=0 ;
a3=0 ;
a4=1 ;
a5=0 ;
a6=0 ;
a7=0 ;

delay(50000);//7
a1=1 ;
a2=1 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;

delay(50000);//8
a1=0 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;

delay(50000);//9
a1=0 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;

	}		
}