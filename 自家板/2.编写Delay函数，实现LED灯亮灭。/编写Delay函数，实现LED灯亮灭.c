#include<reg52.h>
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
void  delay(uint i);
sbit a=P2^0;
sbit b=P2^1;


void main()
{			while(1){
delay(50000);
a=0x00;
b=0x01;	
delay(50000);
a=0x01;
b=0x00;
			 }
}


void  delay(uint i)
{
	while(i--);
}
  