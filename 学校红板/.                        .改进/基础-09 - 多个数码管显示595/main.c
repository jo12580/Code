#include<reg52.h>
#include <74hc595.h>
void  delay(unsigned int i);
sbit LED1=P1^4;
void main()
{   unsigned char  n,j;

	display_date(10,1,12);
	display_temperature(21,15);
     display_time(13,25,30,6);
while(1)
{
    for(j=0;j<10;j++)
	{
	  for(n=0;n<10;n++)
	  {
		HC595SendData(n,1); 
		HC595SendData(j,1); 
		HC595SendData(0,1); 
		HC595SendData(0,1); 

		HC595SendData(0,1); 
		HC595SendData(0,1); 
		HC595SendData(0,1); 
		HC595SendData(0,1); 
	
	RCK_595=0; //set dataline low
	_nop_();  //¶ÌÔÝÑÓÊ±
	_nop_();  //¶ÌÔÝÑÓÊ±
	RCK_595=1;
		LED1= ~LED1;
		delay(40000);
		delay(40000);
	}		
}}}
 
//---------ÑÓÊ±º¯Êý-------------
void  delay(unsigned int i)
{
	while(i--);
}
