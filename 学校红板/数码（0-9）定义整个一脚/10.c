#include "reg52.h"			
typedef unsigned int u16;	 
typedef unsigned char u8;
sbit a1=P0^1; 													 // ˝¬Îπ‹Œª÷√1234  a1 a2 a3 a4
sbit a2=P0^2;													           //5678  a7 a6 a5 a0															  
sbit a3=P0^3;
sbit a4=P0^4;
sbit a5=P0^5;
sbit a6=P0^6;
sbit a7=P0^7;
sbit a0=P0^0;

sbit led=P2^0;		
int b[18]={0x03,0xCF,0x25,0x85,0xC9,0x91,0x11,0xC7,0x01,0x81,0x41,0x19,0x33,0x0D,0x31,0x71,0xFD,0xFE};
int j;
void delay(u16 i)
{
	while(i--);	
}

void main()
{
while (1)
	{
	  for(j=0;j<19;j++)
	  {
	  P0=b[j];
	  delay(50000);
	  }
	}
}