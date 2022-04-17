#include <reg52.h>			
typedef unsigned int u16;	  
typedef unsigned char u8;

typedef unsigned char uchar	  ;	    //延迟定义
typedef unsigned int  uint	;
void  delay(uint i);
								  
sbit LSA=P2^2;					    //数码管
sbit LSB=P2^3;
sbit LSC=P2^4;
unsigned char temp[8]={1,2,3,4,5,6,7,8};

void smg_display(unsigned char *date)


